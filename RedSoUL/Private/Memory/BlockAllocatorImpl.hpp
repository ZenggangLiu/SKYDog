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
#include <unordered_map>
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
    /// Logic页尾部的Block信息
    struct BlockInfo
    {
        /// FreeBlock Mask(BitMask标识Block是否空闲): 1表示Free, 0表示Used
        uint64_t free_block_mask[4];
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        /// Block信息Magic头
        uint32_t magic_id;
        /// 当前Block所在的Logic页索引
        uint32_t page_idx;
#endif
        /// 空闲Block的数目
        uint8_t  free_count;
    };

    /// VM页(OS申请的)信息
    struct VMPageInfo
    {
        /// VM页起始地址
        uint8_t * vm_page_addr;
        /// VM页数目
        uint32_t  vm_page_count;
        /// 使用内存总量(字节数)
        uint32_t  memory_usage;
        /// Logic页数目
        uint32_t  logic_page_count;
    };
    typedef std::vector<VMPageInfo> VMPageInfoListT;

    /// Logic页信息
    struct LogicPageInfo
    {
        /// 所属VMPageInfo的索引
        uint32_t vm_page_info_idx;
        /// 所属LogicPage在VMPage中的索引
        uint32_t logic_page_idx;
    };
    /// <Address, BlockLocationInfo>
    typedef std::unordered_map<uintptr_t, LogicPageInfo> LogicPageInfoTableT;

    bool
    alloc_new_memory_block ();

    /// 根据BlockSize来计算Block的数目
    static
    uint8_t
    calc_block_count_per_page (
        const uint8_t block_size);

    /// 计算BlockInfo的位置
    BlockInfo *
    calc_block_info_addr (
        const VMPageInfo & vm_page_info,
        const uint32_t     logic_page_idx) const;

private:
    /// 所有申请到的VM页信息列表
    VMPageInfoListT     m_vm_page_info_list;
    /// 所有Logic页信息查询表
    LogicPageInfoTableT m_logic_page_info_table;
    /// 当前使用的Logic页数目
    uint32_t            m_logic_page_count;
    /// 每个Logic页中的Block数目
    uint8_t             m_blocks_per_page;
    /// 每个Block的大小(字节数)
    uint8_t             m_block_size;
    /// Memory Block增长率
    uint8_t             m_increment_rate;
};
