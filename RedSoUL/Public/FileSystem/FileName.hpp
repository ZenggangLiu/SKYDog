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
    Created:  19/04/26  @  3:08 PM
    FileName: FileName.hpp @ RedSoUL Project
    History:
             - created by: 19/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...


/// 提供对文件名称的各种操作
///
struct FileName
{
    /// 添加指定扩展名
    /// NOTE: 如果指定的文件名已带有扩展名, 其将被替代
    ///
    /// @param[in]  file_name
    ///     文件名
    /// @param[in]  extension
    ///     扩展名. 例如: "ppm"
    /// @return
    ///     True    如果添加成功
    ///     False   如果添加失败
    static
    bool
    append_file_extension (
        const char * const file_name,
        const char * const extension,
        char * const       buffer_start,
        const uint32_t     buffer_size);
};
