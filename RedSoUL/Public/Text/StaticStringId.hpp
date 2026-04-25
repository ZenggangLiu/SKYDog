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


#if !defined(STRING_HASH)

    /// 计算Hash时使用的32位Seed
    #define STATIC_STRING_HASH_SEED (FOUR_CC_32('S', 'T', 'X', 'T'))

    #if defined(USE_64BITS_ID) /// 使用64位Id类型
        /// 计算Hash值的函数
        #define RUNTIME_STRING_HASH(string) \
            RUNTIME_STRING_HASH_64(STATIC_STRING_HASH_SEED, string)
        #define STATIC_STRING_HASH(string)  \
            STATIC_STRING_HASH_64(STATIC_STRING_HASH_SEED, string)

        /// Static String Id类型
        typedef uint64_t StaticStringIdT;

    #else
        /// 计算Hash值的函数
        #define RUNTIME_STRING_HASH(string) \
            RUNTIME_STRING_HASH_32(STATIC_STRING_HASH_SEED, string)
        #define STATIC_STRING_HASH(string)  \
            STATIC_STRING_HASH_32(STATIC_STRING_HASH_SEED, string)

        /// Static String Id类型
        typedef uint32_t StaticStringIdT;
    #endif /// defined(USE_64BITS_ID)

    /// 非法Static String Id
    #define INVALID_STATIC_STRING_ID ((StaticStringIdT)(-1))

#endif /// !defined(STRING_HASH)

