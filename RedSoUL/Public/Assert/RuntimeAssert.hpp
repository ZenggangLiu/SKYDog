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
    Created:  11/02/26  @  3:44 PM
    FileName: RuntimeAssert.hpp @ RedSoUL Project
    History:
             - created by: 11/02/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


#include "Common/CompilerDefines.hpp" /// BUILD_MODE
#include "DataType/BuiltInTypes.hpp"  /// Bool, ASCII, UInt


#if !defined(RUNTIME_ASSERT)
    #if (BUILD_MODE == DEBUG_BUILD_MODE)
void
RuntimeAssert (
    const Bool          condition,
    const ASCII * const file_name,
    const UInt          line_number,
    const ASCII * const format,
    ...);


        #define RUNTIME_ASSERT(condition, format, ...) \
            RuntimeAssert(        \
                condition,        \
                (ASCII*)__FILE__, \
                (UInt)__LINE__,   \
                format,           \
                ##__VA_ARGS__)
    #else
        #define RUNTIME_ASSERT(condition, format, ...)
    #endif /// (BUILD_MODE == DEBUG_BUILD_MODE)
#endif /// !defined(RUNTIME_ASSERT)
