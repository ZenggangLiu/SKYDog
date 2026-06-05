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


/// Logic页大小(4K)。我们将VM页分(16K)为大小相同的Logic页(4K)
static constexpr uint32_t LOGIC_PAGE_SIZE = BLOCK_ALLOCATOR_PAGE_SIZE;
/// OS内存申请支持的最多Logic页
static constexpr uint32_t MAX_SUPPORTED_LOGIC_PAGE_COUNT = 32u;
static constexpr uint32_t PAGE_INFO_MAGIC_ID = FOUR_CC_32('B', 'A', 'L', 'C');


BlockAllocatorImpl::BlockAllocatorImpl (
    const uint16_t block_size)
:
    m_logic_page_count(0),
    m_blocks_per_page(calc_block_count_per_page(block_size)),
    m_block_size(block_size),
    m_increment_rate(0)
{
    RUNTIME_ASSERT(block_size >= 16 && block_size <= 256 && (block_size & 7) == 0,
                   "Block size must be a multiple of 8 in [16, 256]!!");
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
            logic_page_count <= MAX_SUPPORTED_LOGIC_PAGE_COUNT ?
            logic_page_count                                   :
            MAX_SUPPORTED_LOGIC_PAGE_COUNT;
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
    /// 遍历每一个VM页
    for (auto & vm_page_info : m_vm_page_info_list)
    {
        /// 遍历每一个VM页中的每一个Logic页, 检测是否还有FreeBlock
        for (uint32_t logic_page_idx = 0;
             logic_page_idx < vm_page_info.logic_page_count; ++logic_page_idx)
        {
            BlockInfo * const block_info =
                calc_block_info_addr(vm_page_info, logic_page_idx);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            RUNTIME_ASSERT(block_info->magic_id == PAGE_INFO_MAGIC_ID,
                           "Invalid BlockAllocator logic page info!!");
            RUNTIME_ASSERT(block_info->page_idx == logic_page_idx,
                           "Wrong BlockAllocator logic page index!!");
#endif
            if (block_info->free_count > 0)
            {
                for (uint8_t mask_idx = 0; mask_idx < 4; ++mask_idx)
                {
                    /// 从第一个Block开始, 向后找第一个FreeBlock
                    const uint8_t setbit_pos =
                        BitOperationUtility::bit_scan_forward64(
                            block_info->free_block_mask[mask_idx]);
                    if (setbit_pos != NO_SETBIT_AT_ALL)
                    {
                        block_info->free_block_mask[mask_idx] &= ~((uint64_t)1 << setbit_pos);
                        --block_info->free_count;

                        const uint8_t block_idx = mask_idx * 64 + setbit_pos;
                        uint8_t * const logic_page_start_addr =
                            vm_page_info.vm_page_addr + logic_page_idx * LOGIC_PAGE_SIZE;
                        uint8_t * const block_start_addr =
                            logic_page_start_addr + block_idx * m_block_size;
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
        /// 获取新添加的VM页信息
        VMPageInfo & vm_page_info = m_vm_page_info_list.back();
        /// 获取第一个Block的信息
        BlockInfo * const block_info = calc_block_info_addr(vm_page_info, 0);
        /// 由于所有的Block都为Free, 所以free_block_mask[0] = 0xFFFFFFFFFFFFFFFF
        /// 这里将第一个Block标记为Used, 使用Mask 0xFFFFFFFFFFFFFFFE
        block_info->free_block_mask[0] &= ~((uint64_t)1);
        --block_info->free_count;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        std::memset(vm_page_info.vm_page_addr, 'A', m_block_size);
#endif
        return vm_page_info.vm_page_addr;
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
        /// 判断指定地址是否有效(是否为此Allocator所分配)
        uint8_t * const block_start_addr = (uint8_t*)alloc_addr;
        /// 计算Block所在的LogicPage起始地址
        const uintptr_t logic_page_addr =
            (uintptr_t)block_start_addr & ~((uintptr_t)LOGIC_PAGE_SIZE - 1);

        const auto logic_page_iter = m_logic_page_info_table.find(logic_page_addr);
        /// 非法地址
        if (logic_page_iter == m_logic_page_info_table.end())
        {
            return false;
        }
        else
        {
            const LogicPageInfo & logic_page_info = logic_page_iter->second;
            RUNTIME_ASSERT(logic_page_info.vm_page_info_idx < m_vm_page_info_list.size(),
                           "Wrong BlockAllocator block index!!");

            const VMPageInfo & vm_page_info =
                m_vm_page_info_list[logic_page_info.vm_page_info_idx];
            const uint32_t logic_page_idx = logic_page_info.logic_page_idx;
            BlockInfo * const block_info =
                calc_block_info_addr(vm_page_info, logic_page_idx);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            RUNTIME_ASSERT(block_info->magic_id == PAGE_INFO_MAGIC_ID,
                           "Invalid BlockAllocator logic page info!!");
            RUNTIME_ASSERT(block_info->page_idx == logic_page_idx,
                           "Wrong BlockAllocator logic page index!!");
#endif

            /// 检测地址偏移是否合法:
            /// 1) 不可超越当前Page
            /// 2) 偏移必须为Block的起始位置
            const uint32_t logic_page_offset =
                (uint32_t)(block_start_addr - (uint8_t*)logic_page_addr);
            if ((logic_page_offset < (uint32_t)m_blocks_per_page * m_block_size) &&
                (logic_page_offset % m_block_size) == 0)
            {
                /// 校验当前页的FreeCount
                if (block_info->free_count < m_blocks_per_page)
                {
                    const uint8_t block_idx =
                        (uint8_t)(logic_page_offset / m_block_size);
                    const uint8_t mask_idx = block_idx / 64;
                    const uint8_t bit_pos  = block_idx % 64;
                    const uint64_t block_bit = (uint64_t)1 << bit_pos;

                    /// Double De-Allocate
                    if (block_info->free_block_mask[mask_idx] & block_bit)
                    {
                        return false;
                    }

                    /// 使用BitMask记录此Block重新变为空闲
                    block_info->free_block_mask[mask_idx] |= block_bit;
                    ++block_info->free_count;
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
    else
    {
        return false;
    }
}


void
BlockAllocatorImpl::release ()
{
    for (const auto & vm_page_info : m_vm_page_info_list)
    {
        MemoryUtility::release_vm_pages(
            vm_page_info.vm_page_addr, vm_page_info.vm_page_count);
    }

    m_vm_page_info_list.clear();
    m_logic_page_info_table.clear();
    m_logic_page_count = 0;
    m_blocks_per_page  = calc_block_count_per_page(m_block_size);
    m_increment_rate   = 0;
}


uint16_t
BlockAllocatorImpl::block_size () const
{
    return m_block_size;
}


uint32_t
BlockAllocatorImpl::block_count () const
{
    uint32_t count = 0;
    for (const auto & vm_page_info : m_vm_page_info_list)
    {
        count += vm_page_info.logic_page_count * m_blocks_per_page;
    }
    return count;
}


uint32_t
BlockAllocatorImpl::free_count () const
{
    uint32_t count = 0;
    for (const auto & vm_page_info : m_vm_page_info_list)
    {
        for (uint32_t logic_page_idx = 0;
             logic_page_idx < vm_page_info.logic_page_count; ++logic_page_idx)
        {
            BlockInfo * const block_info =
                calc_block_info_addr(vm_page_info, logic_page_idx);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            RUNTIME_ASSERT(block_info->magic_id == PAGE_INFO_MAGIC_ID,
                           "Invalid BlockAllocator logic page info!!");
            RUNTIME_ASSERT(block_info->page_idx == logic_page_idx,
                           "Wrong BlockAllocator logic page index!!");
#endif
            count += block_info->free_count;
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
    for (const auto & vm_page_info : m_vm_page_info_list)
    {
        memory_usage += vm_page_info.memory_usage;
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
            MathUtility::round_up_count(request_bytes, MemoryUtility::page_size());

        void *   alloc_addr;
        uint32_t alloc_bytes;
        std::tie(alloc_addr, alloc_bytes) =
            MemoryUtility::allocate_vm_pages(vm_page_count);
        if (alloc_addr && alloc_bytes >= LOGIC_PAGE_SIZE)
        {
            VMPageInfo vm_page_info;
            vm_page_info.vm_page_addr     = (uint8_t*)alloc_addr;
            vm_page_info.vm_page_count    = vm_page_count;
            vm_page_info.memory_usage     = alloc_bytes;
            vm_page_info.logic_page_count = alloc_bytes / LOGIC_PAGE_SIZE;

            /// 初始VMPageInfo以及FreeBlock的BitMask
            for (uint32_t logic_page_idx = 0;
                 logic_page_idx < vm_page_info.logic_page_count; ++logic_page_idx)
            {
                BlockInfo * const block_info =
                    calc_block_info_addr(vm_page_info, logic_page_idx);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
                block_info->magic_id = PAGE_INFO_MAGIC_ID;
                block_info->page_idx = logic_page_idx;
#endif
                block_info->free_count = m_blocks_per_page;

                for (uint8_t mask_idx = 0; mask_idx < 4; ++mask_idx)
                {
                    /// 将所有Block都标记为Free: 1表示Free Block
                    block_info->free_block_mask[mask_idx] = ((uint64_t)(-1));
                }
            }

            const uint32_t vm_page_info_idx =
                (uint32_t)m_vm_page_info_list.size();
            m_vm_page_info_list.push_back(vm_page_info);
            m_logic_page_info_table.reserve(
                m_logic_page_info_table.size() + vm_page_info.logic_page_count);
            for (uint32_t logic_page_idx = 0;
                 logic_page_idx < vm_page_info.logic_page_count; ++logic_page_idx)
            {
                const uintptr_t logic_page_addr =
                    (uintptr_t)(vm_page_info.vm_page_addr + logic_page_idx * LOGIC_PAGE_SIZE);
                m_logic_page_info_table[logic_page_addr] = { vm_page_info_idx, logic_page_idx };
            }

            const uint32_t next_logic_page_count =
                m_logic_page_count * m_increment_rate;
            m_logic_page_count =
                next_logic_page_count <= MAX_SUPPORTED_LOGIC_PAGE_COUNT ?
                next_logic_page_count                                   :
                MAX_SUPPORTED_LOGIC_PAGE_COUNT;

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
    const uint16_t block_size)
{
    /// LogicPage布局图:
    /// [ block1, block2, ..., blockn ][ PageInfo: free bitmask + ... ]

    const uint32_t block_count =
        (LOGIC_PAGE_SIZE - (uint32_t)sizeof(BlockInfo)) / (uint32_t)block_size;
    RUNTIME_ASSERT(block_count <= 0xFF, "Block count must fit in uint8_t!!");

    return (uint8_t)block_count;
}


BlockAllocatorImpl::BlockInfo *
BlockAllocatorImpl::calc_block_info_addr (
    const VMPageInfo & vm_page_info,
    const uint32_t     logic_page_idx) const
{
    /// LogicPage布局图:
    /// [ block1, block2, ..., blockn ][ PageInfo: free bitmask + ... ]

    /// 期待的Page尾部(最后一个字节)的偏移
    const uint32_t page_end_offset = (logic_page_idx + 1) * LOGIC_PAGE_SIZE;
    const uint8_t * const page_end_addr =
        vm_page_info.vm_page_addr + page_end_offset;

    return (BlockInfo*)(page_end_addr - sizeof(BlockInfo));
}


// MARK: == ALLOCATOR BRIDGE ==
// +----------------------------------+ //
// |        ALLOCATOR BRIDGE          | //
// +----------------------------------+ //
BlockAllocatorImpl *
BlockAllocatorBridge::create (
    const uint16_t block_size)
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


uint16_t
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
