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
    FileName: StaticHash.hpp @ RedSoUL Project
    History:
             - created by: 7/03/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Hashing/StaticXXHash32.hpp"
#include "Hashing/StaticXXHash64.hpp"


namespace StaticHash_Impl
{

    /// 在编译时获得UTF8编码字符串的长度
    constexpr uint64_t
    static_text_length (
        const uint8_t * const data,
        const uint64_t        idx = 0)
    {
        return (data[idx] == 0) ? idx : static_text_length(data, idx + 1);
    }

}


#if !defined(STATIC_HASH)
    #define STATIC_STRING_LENGTH(text) \
        StaticHash_Impl::static_text_length((const uint8_t*)text)
    /// 可以用在Message Id
    #define STATIC_STRING_HASH_32(seed, string) \
        XXHash32::hash(seed, (const char*)string, (uint32_t)STATIC_STRING_LENGTH(string))
    /// 可以用在String Id
    #define STATIC_STRING_HASH_64(seed, string) \
        XXHash64::hash(seed, (const char*)string, STATIC_STRING_LENGTH(string))
#endif /// !defined(STATIC_HASH)
