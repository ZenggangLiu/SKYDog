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
    Created:  5/08/26  @  4:26 PM
    FileName: LdrColor.hpp @ RedSoUL Project
    History:
             - created by: 5/08/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint8_t, uint32_t


/// Ldr Rgba色彩
///
/// NOTE:
/// LdrColor定义为POD/Aggregate类型
/// - NO constructor, NO copy constructor, NO operator=(),
///   NO protect/private data, NO base class, NO virtual functions
/// - 缺省的opeartor=()将使用std::memcpy()逐字节复制
///
struct LdrColor
{
    static constexpr uint32_t DIMENSION = 4;

    /// 内存分布 [R][G][B][A]
    ///  低位             高位
    ///  +---+---+---+---+
    ///  | R | G | B | A |
    ///  +---+---+---+---+
    union
    {
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };
        uint8_t  e[DIMENSION];
        uint32_t rgba;
    };

    /// 使用指定的数值创建色彩: [r, g, b, a]
    static
    LdrColor
    make (
        const uint8_t r,
        const uint8_t g,
        const uint8_t b,
        const uint8_t a);

    /// 检测当前色彩是否与另一个色彩color相同
    bool
    operator== (
        const LdrColor color) const;

    /// 检测当前色彩是否与另一个色彩color不同
    bool
    operator!= (
        const LdrColor color) const;

    /// 获得指定索引成员的数值
    ///
    /// 例如:
    /// const LdrColor color;
    /// const uint8_t green = color[1];
    uint8_t
    operator[] (
        const uint8_t idx) const;

    /// 设定指定索引成员
    ///
    /// 例如:
    /// LdrColor color;
    /// color[1] = 128;
    uint8_t &
    operator[] (
        const uint8_t idx);
};
