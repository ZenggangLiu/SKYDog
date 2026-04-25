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
    Created:  3/04/26  @  4:38 PM
    FileName: RenderMeshId.hpp @ RedSoUL Project
    History:
             - created by: 3/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...


#if !defined(RENDER_MESH_HASH)

    /// 计算Hash时使用的32位Seed
    #define RENDER_MESH_HASH_SEED (FOUR_CC_32('R', 'N', 'D', 'M'))

    #if defined(USE_64BITS_ID) /// 使用64位Id类型
        /// 计算Hash值的函数
        #define RUNTIME_RENDER_MESH_HASH(unique_name) \
            RUNTIME_STRING_HASH_64(RENDER_MESH_HASH_SEED, unique_name)
        #define STATIC_RENDER_MESH_HASH(unique_name)  \
            STATIC_STRING_HASH_64(RENDER_MESH_HASH_SEED, unique_name)

        /// Mesh Id类型
        typedef uint64_t RenderMeshIdT;

    #else
        /// 计算Hash值的函数
        #define RUNTIME_RENDER_MESH_HASH(unique_name) \
            RUNTIME_STRING_HASH_32(RENDER_MESH_HASH_SEED, unique_name)
        #define STATIC_RENDER_MESH_HASH(unique_name)  \
            STATIC_STRING_HASH_32(RENDER_MESH_HASH_SEED, unique_name)

        /// Mesh Id类型
        typedef uint32_t RenderMeshIdT;
    #endif /// defined(USE_64BITS_ID)

    /// 非法Mesh Id
    #define INVALID_RENDER_MESH_ID ((RenderMeshIdT)(-1))

#endif /// !defined(RENDER_MESH_HASH)
