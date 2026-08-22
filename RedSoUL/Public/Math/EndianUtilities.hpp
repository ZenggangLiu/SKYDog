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
    Created:  20/08/26  @  10:41 PM
    FileName: EndianUtilities.hpp @ RedSoUL Project
    History:
             - created by: 20/08/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint16_t,...


struct EndianUtility
{
    /// 检测当前CPU是否使用Big Endian
    static
    bool
    is_big_endian ();

    /// 交换16位整数的Endian(即: 0xAABB --> 0xBBAA)
    static
    uint16_t
    swap_endian_16 (
        const uint16_t data);

    /// 交换32位整数的Endian(即: 0xAABBCCDD --> 0xDDCCBBAA)
    static
    uint32_t
    swap_endian_32 (
        const uint32_t data);

    /// 交换64位整数的Endian(即: 0xAABBCCDDEEFFGGHH --> 0xHHGGFFEEDDCCBBAA)
    static
    uint64_t
    swap_endian_64 (
        const uint64_t data);
};
