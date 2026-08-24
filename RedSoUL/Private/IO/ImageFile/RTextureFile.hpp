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
    Created:  24/08/26  @  6:51 PM
    FileName: RTextureFile.hpp @ RedSoUL Project
    History:
             - created by: 24/08/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Render/RenderTextureId.hpp"
#include "Render/TextureDataType.hpp"


struct RenderTexture;


/// Render Texture序列化文件
///
/// 文件Layout:
///
/// +--------------------------+
/// | Render Texture File Head |
/// +--------------------------+
/// | Texel Data List          |
/// +--------------------------+
///
struct RTextureFile
{
    /// 读入一个RenderTexture文件
    static
    bool
    read_from (
        const char * const     abs_file_name,
        const RenderTextureIdT exp_texture_id,
        uint32_t &             texture_width,
        uint32_t &             texture_height,
        uint8_t  &             mipmap_count,
        bool &                 is_cube_map,
        bool &                 is_linear_rgb,
        TextureDataType &      texture_data_type,
        uint32_t &             slice_data_size,
        uint8_t * &            texel_list,
        uint32_t &             texel_list_size);

    /// 创建一个RenderTexture文件
    ///
    /// @param[in]  abs_file_name
    ///     绝对文件路径
    ///     NOTE: .rtexture文件扩展符将添加到指定的路径名上
    /// @param[in]  texture_data
    ///     Texture数据
    /// @return
    ///     True:   RenderTexture文件输出成功
    ///     False:  RenderTexture文件输出失败
    static
    bool
    write_to (
        const char * const    abs_file_name,
        const RenderTexture & texture_data);
};
