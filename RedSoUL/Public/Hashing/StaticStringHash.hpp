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
    Created:  7/03/26  @  12:00 PM
    FileName: StaticStringHash.hpp @ RedSoUL Project
    History:
             - created by: 7/03/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Hashing/StaticXXHash32.hpp"
#include "Hashing/StaticXXHash64.hpp"


namespace StaticStringHash_Impl
{

    /// 在编译时获得UTF8编码字符串的长度
    constexpr uint32_t
    static_text_length (
        const char * const data,
        const uint32_t     idx = 0)
    {
        return (data[idx] == 0) ? idx : static_text_length(data, idx + 1);
    }

}


#if !defined(STATIC_STRING_HASH)
    /// 可以用在Message Id
    #define STATIC_STRING_HASH_32(seed, string) \
        XXHash32::hash(seed, string, StaticStringHash_Impl::static_text_length(string))
    /// 可以用在String Id
    #define STATIC_STRING_HASH_64(seed, string) \
        XXHash64::hash(seed, string, StaticStringHash_Impl::static_text_length(string))
#endif /// !defined(STATIC_STRING_HASH)
