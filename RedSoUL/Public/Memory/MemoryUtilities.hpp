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
    Created:  16/02/26  @  10:37 PM
    FileName: MemoryUtilities.hpp @ RedSoUL Project
    History:
             - created by: 16/02/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


#include <tuple>
#include "DataType/BuiltInTypes.hpp"


struct MemoryUtility
{
    /// 获得此进程/程序的内存使用情况
    ///
    /// @return
    ///      如果申请成功：<- [ true,  使用的物理内存(字节), 使用的虚拟内存(字节) ]
    ///      如果申请失败：<- [ false, 0,                 0                ]
    static
    std::tuple<Bool, ULong, ULong>
    memory_usage ();

    /// 获得操作系统(OS)提供的内存页长度(字节数)
    static
    UInt
    page_size ();

    /// 申请指定个数目的内存页(虚拟内存页)
    ///
    /// @param[in]  page_count
    ///      内存页的数目
    /// @return
    ///      如果申请成功：<- [ 申请的内存地址, 申请的内存大 ]
    ///      如果申请失败：<- [ nullptr,     0          ]
    /// @example:
    ///     例如：申请16个虚拟内存页：
    ///     const void * alloc_addr; UInt alloc_size;
    ///     std::tie(alloc_addr, alloc_size) = MemoryUtility::allocate_vm_pages(16);
    static
    std::tuple<void*, UInt>
    allocate_vm_pages (
        const UInt page_count);

    /// 释放指定的内存页(虚拟内存页)
    ///
    /// @param[in]  alloc_addr
    ///      申请到的起始内存地址
    /// @param[in]  page_count
    ///      内存页的数目
    /// @return
    ///      TRUE:  如果内存释放成功
    ///      FALSE: 如果失败
    static
    Bool
    release_vm_pages (
        void * const alloc_addr,
        const UInt   page_count);
};
