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
    Created:  5/08/26  @  4:25 PM
    FileName: HdrColor.hpp @ RedSoUL Project
    History:
             - created by: 5/08/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t
/// Library headers
#include "DataType/Half.hpp"


/// Hdr Rgba色彩
///
/// 注意:
/// HdrColor是POD/聚合类型
///
struct HdrColor
{
    static constexpr uint32_t DIMENSION = 4;

    /// 内存布局 [R][G][B][A]
    ///  低位             高位
    ///  +---+---+---+---+
    ///  | R | G | B | A |
    ///  +---+---+---+---+
    union
    {
        struct
        {
            half r;
            half g;
            half b;
            half a;
        };
        half e[DIMENSION];
    };

    static
    HdrColor
    make (
        const float r,
        const float g,
        const float b,
        const float a);

    static
    HdrColor
    make (
        const half r,
        const half g,
        const half b,
        const half a);

    /// 检测当前色彩是否与另一个色彩color相同
    bool
    operator== (
        const HdrColor color) const;

    /// 检测当前色彩是否与另一个色彩color不同
    bool
    operator!= (
        const HdrColor color) const;

    /// 获得指定索引成员的数值
    ///
    /// 例如:
    /// const HdrColor color;
    /// const half green = color[1];
    half
    operator[] (
        const uint8_t idx) const;

    /// 设定指定索引成员
    ///
    /// 例如:
    /// HdrColor color;
    /// color[1] = half::make(2.0f);
    half &
    operator[] (
        const uint8_t idx);
};
