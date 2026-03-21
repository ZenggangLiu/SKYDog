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
    Created:  27/02/26  @  10:51 PM
    FileName: StaticStringId.hpp @ RedSoUL Project
    History:
             - created by: 27/02/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Common/PlatformDefines.hpp" /// OS_TYPE


#if !defined(RUNTIME_STRING_HASH)
    /// 定义RUNTIME_STRING_HASH()/STATIC_STRING_HASH()来计算字符串的Hash值
    #if (OS_TYPE == OS_TYPE_IOS) || (OS_TYPE == OS_TYPE_TVOS) /// iOS我们使用32位String Id
        #define RUNTIME_STRING_HASH(seed, string) RUNTIME_STRING_HASH_32(seed, string)
        #define STATIC_STRING_HASH(seed,  string) STATIC_STRING_HASH_32(seed,  string)
        typedef uint32_t StaticStringIdT;

    #else /// macOS, Linux, Windows我们使用64位String Id
        #define RUNTIME_STRING_HASH(seed, string) RUNTIME_STRING_HASH_64(seed, string)
        #define STATIC_STRING_HASH(seed,  string) STATIC_STRING_HASH_64(seed,  string)
        typedef uint64_t StaticStringIdT;
    #endif /// (OS_TYPE == OS_TYPE_IOS) || (OS_TYPE == OS_TYPE_TVOS)

    #define INVALID_STATIC_STRING_ID ((StaticStringIdT)(-1))
#endif /// !defined(RUNTIME_STRING_HASH)

