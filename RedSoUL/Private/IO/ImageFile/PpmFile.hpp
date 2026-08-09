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
    Created:  21/04/26  @  10:40 PM
    FileName: PpmFile.hpp @ RedSoUL Project
    History:
             - created by: 21/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...


struct LdrColor;


/// PPM(Portable Pixel Format)文件
///
struct PpmFile
{
    /// 创建一个24位RGB PPM文件
    ///
    /// @param[in]  abs_file_name
    ///     绝对文件路径
    ///     NOTE: .ppm文件扩展符将添加到指定的路径名上
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
    /// @param[in]  is_rgba_layout
    ///     Pixel数据是否以 RGBA 的顺序存储
    ///     - YES: 存储顺序如下:
    ///     -      低位             高位
    ///     -      +---+---+---+---+
    ///     -      | R | G | B | A |
    ///     -      +---+---+---+---+
    ///     - NO:  存储顺序如下:
    ///     -      低位             高位
    ///     -      +---+---+---+---+
    ///     -      | B | G | R | A |
    ///     -      +---+---+---+---+
    /// @param[in]  use_alpha
    ///     表示是否使用Alpha通道
    /// @param[in]  use_binary
    ///     是否使用二进制格式存储
    /// @return
    ///     True:   PPM文件输出成功
    ///     False:  PPM文件输出失败
    static
    bool
    write_to (
        const char * const     abs_file_name,
        const uint32_t         image_width,
        const uint32_t         image_height,
        const LdrColor * const pixel_array,
        const uint32_t         pixel_count,
        const bool             is_rgba_layout,
        const bool             use_alpha,
        const bool             use_binary);
};
