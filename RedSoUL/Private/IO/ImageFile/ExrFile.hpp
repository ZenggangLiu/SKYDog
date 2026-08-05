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
    Created:  5/08/26  @  4:29 PM
    FileName: ExrFile.hpp @ RedSoUL Project
    History:
             - created by: 5/08/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...


struct HdrColor;


/// EXR(Extended High Dynamic Range Format)文件
///
struct ExrFile
{
    /// 创建一个64位EXR文件
    ///
    /// @param[in]  abs_file_name
    ///     绝对文件路径
    ///     NOTE: .exr文件扩展符将添加到给定的路径名上
    /// @param[in]  image_width
    ///     图形的宽度(Pixel)
    /// @param[in]  image_height
    ///     图形的高度(Pixel)
    /// @param[in]  pixel_array
    ///     Pixel数组的起始地址
    /// Pixel数据使用如下坐标系:
    ///
    ///   ^ 上方
    ///   |
    ///   +-------------+--------------+
    ///   | First Pixel | Second Pixel |  <----- 第二行
    ///   |      3      |      4       |
    ///   +-------------+--------------+
    ///   | First Pixel | Second Pixel |  <----- 第一行
    ///   |      1      |      2       |
    /// O +-------------+--------------+----> 右侧
    /// @param[in]  pixel_count
    ///     Pixel的个数
    /// @return
    ///     True:   EXR文件输出成功
    ///     False:  EXR文件输出失败
    ///
    /// NOTE:
    /// Exr文件没有Embedded Color Profile。
    /// 我们在此文件中存储Linear Color Space的色彩
    static
    bool
    write_to (
        const char * const     abs_file_name,
        const uint32_t         image_width,
        const uint32_t         image_height,
        const HdrColor * const pixel_array,
        const uint32_t         pixel_count);
};
