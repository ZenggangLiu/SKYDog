/// System headers
#include <cstring> /// std::memset
#include <new>
#include <tuple>
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CommonDefines.hpp"
#include "Math/BitOperationUtilities.hpp"
#include "Math/MathUtilities.hpp"
#include "Memory/BlockAllocator.hpp"
#include "Memory/MemoryUtilities.hpp"
/// Self header
#include "Memory/BlockAllocatorImpl.hpp"


/// 逻辑页大小(4K)。我们将VM页分(16K)为大小相同的逻辑页(4K)
static constexpr uint32_t LOGIC_PAGE_SIZE = 4096u;
static constexpr uint32_t MAX_ALLOWED_LOGIC_PAGE_COUNT = 16u;
static constexpr uint32_t PAGE_INFO_MAGIC = FOUR_CC_32('B', 'L', 'K', 'A');


BlockAllocatorImpl::BlockAllocatorImpl (
    const uint8_t block_size)
:
    m_logic_page_count(0),
    m_blocks_per_page(calc_block_count_per_page(block_size)),
    m_block_size(block_size),
    m_increment_rate(0)
{
    RUNTIME_ASSERT(block_size >= 16 && block_size <= 128 && (block_size & 7) == 0,
                   "Block size must be a multiple of 8 in [16, 128]!!");
}


BlockAllocatorImpl::~BlockAllocatorImpl ()
{
    release();
}


bool
BlockAllocatorImpl::initialize (
    const uint32_t logic_page_count,
    const uint8_t  increment_rate)
{
    RUNTIME_ASSERT(logic_page_count > 0, "Page count must be larger than zero!!");
    RUNTIME_ASSERT(increment_rate > 0, "Increment rate must be larger than zero!!");
    RUNTIME_ASSERT(MemoryUtility::page_size() >= LOGIC_PAGE_SIZE,
                   "OS page size must be at least 4KB");
    RUNTIME_ASSERT((MemoryUtility::page_size() % LOGIC_PAGE_SIZE) == 0,
                   "OS page size must be a multiple of 4KB");

    if (logic_page_count > 0 && increment_rate > 0)
    {
        release();

        m_logic_page_count =
            logic_page_count <= MAX_ALLOWED_LOGIC_PAGE_COUNT ?
            logic_page_count                                 :
            MAX_ALLOWED_LOGIC_PAGE_COUNT;
        m_increment_rate   = increment_rate;

        return alloc_new_memory_block();
    }
    else
    {
        return false;
    }
}


uint8_t *
BlockAllocatorImpl::allocate ()
{
    /// 遍历每一个Logic页, 检测是否还有FreeBlock
    for (auto & memory_block : m_memory_block_list)
    {
        for (uint32_t page_idx = 0;
             page_idx < memory_block.logic_page_count; ++page_idx)
        {
            LogicPageInfo * const page_info =
                calc_page_info_addr(memory_block, page_idx);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            RUNTIME_ASSERT(page_info->magic_id == PAGE_INFO_MAGIC,
                           "Invalid BlockAllocator logic page info!!");
            RUNTIME_ASSERT(page_info->page_index == page_idx,
                           "Wrong BlockAllocator logic page index!!");
#endif
            if (page_info->free_count > 0)
            {
                for (uint8_t mask_idx = 0; mask_idx < 4; ++mask_idx)
                {
                    const uint8_t setbit_pos =
                        BitOperationUtility::bit_scan_forward64(
                            page_info->free_block_mask[mask_idx]);
                    /// 找到一个FreeBlock
                    if (setbit_pos != NO_SETBIT_AT_ALL)
                    {
                        page_info->free_block_mask[mask_idx] &= ~((uint64_t)1 << setbit_pos);
                        --page_info->free_count;

                        const uint8_t block_idx = mask_idx * 64 + setbit_pos;
                        uint8_t * const page_start_addr =
                            memory_block.vm_page_addr + page_idx * LOGIC_PAGE_SIZE;
                        uint8_t * const block_start_addr =
                            page_start_addr + block_idx * m_block_size;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
                        std::memset(block_start_addr, 'A', m_block_size);
#endif
                        return block_start_addr;
                    }
                }
            }
        }
    }

    /// 如无FreeBlock, 按增长率申请新的空间
    if (alloc_new_memory_block())
    {
        /// 取新添加的Memory Block信息
        MemoryBlock & memory_block = m_memory_block_list.back();
        LogicPageInfo * const page_info = calc_page_info_addr(memory_block, 0);
        /// 由于所有的Block都Free： free_block_mask[0]为0xFFFFFFFFFFFFFFFF
        /// 这里将第一个Block标记为Used: 使用Mask 0xFFFFFFFFFFFFFFFE
        page_info->free_block_mask[0] &= ~((uint64_t)1);
        --page_info->free_count;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        std::memset(memory_block.vm_page_addr, 'A', m_block_size);
#endif
        return memory_block.vm_page_addr;
    }
    /// 无法申请空间
    else
    {
        return nullptr;
    }
}


bool
BlockAllocatorImpl::deallocate (
    void * const alloc_addr)
{
    if (alloc_addr)
    {
        /// 判断给定地址是否有效(是否为此Allocator所分配)
        uint8_t * const alloc_ptr = (uint8_t*)alloc_addr;
        for (auto & memory_block : m_memory_block_list)
        {
            if (alloc_ptr >= memory_block.vm_page_addr)
            {
                const void * const page_end_addr =
                    memory_block.vm_page_addr +
                    memory_block.logic_page_count * LOGIC_PAGE_SIZE;
                if (alloc_ptr < page_end_addr)
                {
                    const uint32_t alloc_offset =
                        (uint32_t)(alloc_ptr - memory_block.vm_page_addr);
                    const uint32_t page_index  = alloc_offset / LOGIC_PAGE_SIZE;
                    const uint32_t page_offset = alloc_offset % LOGIC_PAGE_SIZE;
                    LogicPageInfo * const page_info =
                        calc_page_info_addr(memory_block, page_index);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
                    RUNTIME_ASSERT(page_info->magic_id == PAGE_INFO_MAGIC,
                                   "Invalid BlockAllocator logic page info!!");
                    RUNTIME_ASSERT(page_info->page_index == page_index,
                                   "Wrong BlockAllocator logic page index!!");
#endif

                    /// 检测地址偏移是否合法:
                    /// 1) 不可超越当前Page
                    /// 2) 偏移必须为Block的起始位置
                    if ((page_offset < (uint32_t)m_blocks_per_page * m_block_size) &&
                        (page_offset % m_block_size) == 0)
                    {
                        /// 校验当前页的FreeCount
                        if (page_info->free_count < m_blocks_per_page)
                        {
                            const uint8_t block_idx =
                                (uint8_t)(page_offset / m_block_size);
                            const uint8_t mask_idx = block_idx / 64;
                            const uint8_t bit_pos  = block_idx % 64;
                            const uint64_t block_bit = (uint64_t)1 << bit_pos;

                            /// Double De-Allocate
                            if (page_info->free_block_mask[mask_idx] & block_bit)
                            {
                                return false;
                            }

                            /// 使用BitMask记录此Block重新变为空闲
                            page_info->free_block_mask[mask_idx] |= block_bit;
                            ++page_info->free_count;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
                            std::memset(alloc_addr, 'D', m_block_size);
#endif
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}


void
BlockAllocatorImpl::release ()
{
    for (const auto & memory_block : m_memory_block_list)
    {
        MemoryUtility::release_vm_pages(
            memory_block.vm_page_addr, memory_block.vm_page_count);
    }

    m_memory_block_list.clear();
    m_logic_page_count = 0;
    m_blocks_per_page  = calc_block_count_per_page(m_block_size);
    m_increment_rate   = 0;
}


uint8_t
BlockAllocatorImpl::block_size () const
{
    return m_block_size;
}


uint32_t
BlockAllocatorImpl::block_count () const
{
    uint32_t count = 0;
    for (const auto & memory_block : m_memory_block_list)
    {
        count += memory_block.logic_page_count * m_blocks_per_page;
    }
    return count;
}


uint32_t
BlockAllocatorImpl::free_count () const
{
    uint32_t count = 0;
    for (const auto & memory_block : m_memory_block_list)
    {
        for (uint32_t idx = 0; idx < memory_block.logic_page_count; ++idx)
        {
            LogicPageInfo * const page_info = calc_page_info_addr(memory_block, idx);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            RUNTIME_ASSERT(page_info->magic_id == PAGE_INFO_MAGIC,
                           "Invalid BlockAllocator logic page info!!");
            RUNTIME_ASSERT(page_info->page_index == idx,
                           "Wrong BlockAllocator logic page index!!");
#endif
            count += page_info->free_count;
        }
    }
    return count;
}


uint32_t
BlockAllocatorImpl::used_count () const
{
    return block_count() - free_count();
}


uint32_t
BlockAllocatorImpl::allocated_bytes () const
{
    uint32_t memory_usage = 0;
    for (const auto & memory_block : m_memory_block_list)
    {
        memory_usage += memory_block.memory_usage;
    }
    return memory_usage;
}


bool
BlockAllocatorImpl::alloc_new_memory_block ()
{
    RUNTIME_ASSERT(m_logic_page_count > 0, "Page count must be larger than zero!!");
    RUNTIME_ASSERT(m_logic_page_count <= 0xFFFFFFFFu / LOGIC_PAGE_SIZE,
                   "Page count is too large!!");

    if (m_logic_page_count)
    {
        const uint32_t request_bytes = m_logic_page_count * LOGIC_PAGE_SIZE;
        const uint32_t vm_page_count =
            MathUtility::multiple_of(request_bytes, MemoryUtility::page_size());

        void *   alloc_addr;
        uint32_t alloc_bytes;
        std::tie(alloc_addr, alloc_bytes) =
            MemoryUtility::allocate_vm_pages(vm_page_count);
        if (alloc_addr && alloc_bytes >= LOGIC_PAGE_SIZE)
        {
            MemoryBlock memory_block;
            memory_block.vm_page_addr     = (uint8_t*)alloc_addr;
            memory_block.vm_page_count    = vm_page_count;
            memory_block.memory_usage     = alloc_bytes;
            memory_block.logic_page_count = alloc_bytes / LOGIC_PAGE_SIZE;

            /// 初始PageInfo以及FreeBlock的BitMask
            for (uint32_t page_idx = 0;
                 page_idx < memory_block.logic_page_count; ++page_idx)
            {
                LogicPageInfo * const page_info =
                    calc_page_info_addr(memory_block, page_idx);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
                page_info->magic_id   = PAGE_INFO_MAGIC;
                page_info->page_index = page_idx;
#endif
                page_info->free_count = m_blocks_per_page;

                for (uint8_t mask_idx = 0; mask_idx < 4; ++mask_idx)
                {
                    /// 将所有Block都标记为Free: 1表示Free Block
                    page_info->free_block_mask[mask_idx] = ((uint64_t)(-1));
                }
            }

            m_memory_block_list.push_back(memory_block);
            const uint32_t next_logic_page_count =
                m_logic_page_count * m_increment_rate;
            m_logic_page_count =
                next_logic_page_count <= MAX_ALLOWED_LOGIC_PAGE_COUNT ?
                next_logic_page_count                                 :
                MAX_ALLOWED_LOGIC_PAGE_COUNT;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


uint8_t
BlockAllocatorImpl::calc_block_count_per_page (
    const uint8_t block_size)
{
    /// LogicPage布局图:
    /// [ block1, block2, ..., blockn ][ PageInfo: free bitmask + ... ]

    const uint32_t block_count =
        (LOGIC_PAGE_SIZE - (uint32_t)sizeof(LogicPageInfo)) / (uint32_t)block_size;
    RUNTIME_ASSERT(block_count <= 0xFF, "Block count must fit in uint8_t!!");

    return (uint8_t)block_count;
}


BlockAllocatorImpl::LogicPageInfo *
BlockAllocatorImpl::calc_page_info_addr (
    const MemoryBlock & memory_block,
    const uint32_t      page_idx) const
{
    /// LogicPage布局图:
    /// [ block1, block2, ..., blockn ][ PageInfo: free bitmask + ... ]

    /// 期待的Page尾部(最后一个字节)的偏移
    const uint32_t page_end_offset = (page_idx + 1) * LOGIC_PAGE_SIZE;
    const uint8_t * const page_end_addr =
        memory_block.vm_page_addr + page_end_offset;

    return (LogicPageInfo*)(page_end_addr - sizeof(LogicPageInfo));
}


// MARK: == ALLOCATOR BRIDGE ==
// +----------------------------------+ //
// |        ALLOCATOR BRIDGE          | //
// +----------------------------------+ //
BlockAllocatorImpl *
BlockAllocatorBridge::create (
    const uint8_t block_size)
{
    return new (std::nothrow) BlockAllocatorImpl(block_size);
}


void
BlockAllocatorBridge::destroy (
    BlockAllocatorImpl * & allocator)
{
    delete allocator;
    allocator = nullptr;
}


bool
BlockAllocatorBridge::initialize (
    BlockAllocatorImpl * const allocator,
    const uint32_t             logic_page_count,
    const uint8_t              increment_rate)
{
    return allocator ? allocator->initialize(logic_page_count, increment_rate) : false;
}


uint8_t *
BlockAllocatorBridge::allocate (
    BlockAllocatorImpl * const allocator)
{
    return allocator ? allocator->allocate() : nullptr;
}


bool
BlockAllocatorBridge::deallocate (
    BlockAllocatorImpl * const allocator,
    void * const               alloc_addr)
{
    return allocator ? allocator->deallocate(alloc_addr) : false;
}


void
BlockAllocatorBridge::release (
    BlockAllocatorImpl * const allocator)
{
    if (allocator)
    {
        allocator->release();
    }
}


uint8_t
BlockAllocatorBridge::block_size (
    const BlockAllocatorImpl * const allocator)
{
    return allocator ? allocator->block_size() : 0;
}


uint32_t
BlockAllocatorBridge::block_count (
    const BlockAllocatorImpl * const allocator)
{
    return allocator ? allocator->block_count() : 0;
}


uint32_t
BlockAllocatorBridge::free_count (
    const BlockAllocatorImpl * const allocator)
{
    return allocator ? allocator->free_count() : 0;
}


uint32_t
BlockAllocatorBridge::used_count (
    const BlockAllocatorImpl * const allocator)
{
    return allocator ? allocator->used_count() : 0;
}


uint32_t
BlockAllocatorBridge::allocated_bytes (
    const BlockAllocatorImpl * const allocator)
{
    return allocator ? allocator->allocated_bytes() : 0;
}
