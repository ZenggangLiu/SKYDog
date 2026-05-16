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
    Created:  16/05/26  @  3:47 PM
    FileName: BlockAllocatorImpl.hpp @ RedSoUL Project
    History:
             - created by: 16/05/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint8_t, uint32_t,...
#include <vector>
/// Library headers
#include "Common/CompilerDefines.hpp" /// BUILD_MODE


/// 内存Block分配器的实现(以相同大小的Block的方式进行内存分配)
class BlockAllocatorImpl
{
public:
    explicit
    BlockAllocatorImpl (
        const uint8_t block_size);

    ~BlockAllocatorImpl ();

    bool
    initialize (
        const uint32_t logic_page_count,
        const uint8_t  increment_rate);

    uint8_t *
    allocate ();

    bool
    deallocate (
        void * const alloc_addr);

    void
    release ();

    uint8_t
    block_size () const;

    uint32_t
    block_count () const;

    uint32_t
    free_count () const;

    uint32_t
    used_count () const;

    uint32_t
    allocated_bytes () const;

private:
    /// 放置在每个Logic页尾部的INFO
    struct LogicPageInfo
    {
        /// 使用BitMask标识Block是否空闲: 1表示Free, 0表示Used
        uint64_t free_block_mask[4];
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        uint32_t magic_id;
        uint32_t page_index;
#endif
        /// 空闲Block的数目
        uint8_t  free_count;
    };

    /// 虚拟Page信息
    struct MemoryBlock
    {
        /// 虚拟页(OS申请的)起始地址
        uint8_t * vm_page_addr;
        /// 虚拟页(OS申请的)数目
        uint32_t  vm_page_count;
        /// 使用内存总量(字节数)
        uint32_t  memory_usage;
        /// 逻辑页数目
        uint32_t  logic_page_count;
    };
    typedef std::vector<MemoryBlock> MemoryBlockListT;

    bool
    alloc_new_memory_block ();

    /// 根据BlockSize来计算Block的数目
    static
    uint8_t
    calc_block_count_per_page (
        const uint8_t block_size);

    /// 计算LogicPageInfo的位置
    LogicPageInfo *
    calc_page_info_addr (
        const MemoryBlock & memory_block,
        const uint32_t      page_idx) const;

private:
    /// 所有申请到的Memory Block
    MemoryBlockListT m_memory_block_list;
    /// 当前使用的Logic页数目
    uint32_t         m_logic_page_count;
    /// 每个Logic页中的Block数目
    uint8_t          m_blocks_per_page;
    /// 每个Block的大小(字节数)
    uint8_t          m_block_size;
    /// Memory Block增长率
    uint8_t          m_increment_rate;
};
