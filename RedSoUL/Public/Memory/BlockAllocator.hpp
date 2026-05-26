/***************************************************************************************
                                                                                        
        *          .               *                              .               *     
        ███████╗██╗  ██╗██╗   ██╗        ██████╗  ██████╗  ██████╗         *            
        ██╔════╝██║ ██╔╝╚██╗ ██╔╝        ██╔══██╗██╔═══██╗██╔════╝                      
        ███████╗█████╔╝  ╚████╔╝         ██║  ██║██║   ██║██║  ███╗        .            
        ╚════██║██╔═██╗   ╚██╔╝          ██║  ██║██║   ██║██║   ██║                     
        ███████║██║  ██╗    ██║           ██████╔╝╚██████╔╝╚██████╔╝         *          
        ╚══════╝╚═╝  ╚═╝    ╚═╝           ╚═════╝  ╚═════╝  ╚═════╝                     
                                                                                        
        <~~~               .        SKY Dog Game                      ~~~>        *     
                                Real-Time | Cross-Platform           .                  
----------------------------------------------------------------------------------------
                                                                                        
                                  ,,                                                    
                  __           o-°°|\_____/)                                            
    Author:   (___()'`; Zee...  \_/|_)     )                                            
              /,    /`             \  __  /                                             
              \\"--\\              (_/ (_/                                              
    Created:  15/05/26  @  10:54 PM
    FileName: BlockAllocator.hpp @ RedSoUL Project
    History:
             - created by: 15/05/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint8_t, uint32_t,...
/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION


class BlockAllocatorImpl;


/// BlockAllocator中内存页的大小(4K｜4096字节)
static constexpr uint32_t BLOCK_ALLOCATOR_PAGE_SIZE = 4096u;


/// 内存Block分配器(以相同大小的Block的方式进行内存分配)
/// NOTE:
/// - Block大小范围为[16字节，128字节], 之间的任意8字节的倍数:
///   [16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128]
///
template <typename ClassType>
class BlockAllocator
{
    static constexpr uint32_t BITS_MASK = ~((uint32_t)7);
    /// round up到的下一个8的倍数
    static constexpr uint32_t CLASS_SIZE_ROUNDED_UP = (uint32_t)sizeof(ClassType) + 7;
    /// 使用的Block大小(字节数)
    static constexpr uint8_t BLOCK_SIZE_USED =
        (CLASS_SIZE_ROUNDED_UP & BITS_MASK) < 16 ?
        16                                       :
        (uint8_t)(CLASS_SIZE_ROUNDED_UP & BITS_MASK);

    static_assert(sizeof(ClassType) <= 128,
                  "The given class is too large, "
                  "BlockAllocator can at most allocate 128 bytes!!");
    static_assert(BLOCK_SIZE_USED <= 128,
                  "The block size is too large, "
                  "BlockAllocator can at most allocate 128 bytes!!");

public:
    INLINE_FUNCTION
     BlockAllocator ();

    INLINE_FUNCTION
    ~BlockAllocator ();

    /// 初始化Allocator
    ///
    /// @param[in]  page_count
    ///     初始的内存页数目(逻辑内存页的大小为4K: 4096字节)
    /// @param[in]  increment_rate
    ///     递增比率
    ///     例如：设定为2，则表示：
    ///           如果当前没有空闲Block时, 需要申请新的内存，则申请两倍当前页数目的空间
    INLINE_FUNCTION
    bool
    initialize (
        const uint32_t page_count = 1,
        const uint8_t  increment_rate = 2);

    /// 申请一个Block
    INLINE_FUNCTION
    uint8_t *
    allocate ();

    /// 释放一个Block
    INLINE_FUNCTION
    bool
    deallocate (
        void * const alloc_addr);

    /// 释放当前Allocator占用的所有内存
    INLINE_FUNCTION
    void
    release ();

    /// 获取Block大小(字节数)
    INLINE_FUNCTION
    uint8_t
    block_size () const;

    /// 获取Block的总数
    INLINE_FUNCTION
    uint32_t
    block_count () const;

    /// 获取FreeBlock数目
    INLINE_FUNCTION
    uint32_t
    free_count () const;

    /// 获取UsedBlock数目
    INLINE_FUNCTION
    uint32_t
    used_count () const;

    /// 获取占用的内存大小(字节数)
    INLINE_FUNCTION
    uint32_t
    allocated_bytes () const;


private:
    BlockAllocator (
        const BlockAllocator &) = delete;
    BlockAllocator & operator = (
        const BlockAllocator &) = delete;

private:
    /// 真是的Allocator实例
    BlockAllocatorImpl * m_allocator;
};


/// BlockAllocator与BlockAllocator具体实现之间的桥梁
///
struct BlockAllocatorBridge
{
    /// 创建Allocator的具体实现
    static
    BlockAllocatorImpl *
    create (
        const uint8_t block_size);

    static
    void
    destroy (
        BlockAllocatorImpl * & allocator);

    static
    bool
    initialize (
        BlockAllocatorImpl * const allocator,
        const uint32_t             page_count,
        const uint8_t              increment_rate);

    /// 申请一个Block
    static
    uint8_t *
    allocate (
        BlockAllocatorImpl * const allocator);

    /// 释放一个Block
    static
    bool
    deallocate (
        BlockAllocatorImpl * const allocator,
        void * const               alloc_addr);

    static
    void
    release (
        BlockAllocatorImpl * const allocator);

    static
    uint8_t
    block_size (
        const BlockAllocatorImpl * const allocator);

    static
    uint32_t
    block_count (
        const BlockAllocatorImpl * const allocator);

    static
    uint32_t
    free_count (
        const BlockAllocatorImpl * const allocator);

    static
    uint32_t
    used_count (
        const BlockAllocatorImpl * const allocator);

    static
    uint32_t
    allocated_bytes (
        const BlockAllocatorImpl * const allocator);
};



template <typename ClassType>
INLINE_FUNCTION
BlockAllocator<ClassType>::BlockAllocator ()
:
    m_allocator(BlockAllocatorBridge::create(BLOCK_SIZE_USED))
{

}


template <typename ClassType>
INLINE_FUNCTION
BlockAllocator<ClassType>::~BlockAllocator ()
{
    BlockAllocatorBridge::destroy(m_allocator);
}


template <typename ClassType>
INLINE_FUNCTION
bool
BlockAllocator<ClassType>::initialize (
    const uint32_t page_count,
    const uint8_t  increment_rate)
{
    return BlockAllocatorBridge::initialize(
        m_allocator, page_count, increment_rate);
}


template <typename ClassType>
INLINE_FUNCTION
uint8_t *
BlockAllocator<ClassType>::allocate ()
{
    return BlockAllocatorBridge::allocate(m_allocator);
}


template <typename ClassType>
INLINE_FUNCTION
bool
BlockAllocator<ClassType>::deallocate (
    void * const alloc_addr)
{
    return BlockAllocatorBridge::deallocate(m_allocator, alloc_addr);
}


template <typename ClassType>
INLINE_FUNCTION
void
BlockAllocator<ClassType>::release ()
{
    BlockAllocatorBridge::release(m_allocator);
}


template <typename ClassType>
INLINE_FUNCTION
uint8_t
BlockAllocator<ClassType>::block_size () const
{
    return BlockAllocatorBridge::block_size(m_allocator);
}


template <typename ClassType>
INLINE_FUNCTION
uint32_t
BlockAllocator<ClassType>::block_count () const
{
    return BlockAllocatorBridge::block_count(m_allocator);
}


template <typename ClassType>
INLINE_FUNCTION
uint32_t
BlockAllocator<ClassType>::free_count () const
{
    return BlockAllocatorBridge::free_count(m_allocator);
}


template <typename ClassType>
INLINE_FUNCTION
uint32_t
BlockAllocator<ClassType>::used_count () const
{
    return BlockAllocatorBridge::used_count(m_allocator);
}


template <typename ClassType>
INLINE_FUNCTION
uint32_t
BlockAllocator<ClassType>::allocated_bytes () const
{
    return BlockAllocatorBridge::allocated_bytes(m_allocator);
}
