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
    Created:  20/08/26  @  10:31 PM
    FileName: HalfUtilities.hpp @ RedSoUL Project
    History:
             - created by: 20/08/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t
/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION
#include "DataType/Half.hpp"
#include "Math/MathUtilities.hpp"


// MARK: == 函数定义 ==
struct HalfUtility
{
    /// Round到最近的那个half数值
    /// NOTE: 如果正好位于正中间, 我们看左右两侧的half哪个为偶数(即: 最低位为0)
    /// - 这样可以使ROUND的误差平均, 以达到最小
    /// - 与IEEE 754行为一致
    INLINE_FUNCTION
    static
    uint32_t
    round_to_nearest_and_tie_to_even (
        const uint32_t value,
        const uint32_t shift);

    /// 将指定的float浮点数转换为half浮点数
    static
    INLINE_FUNCTION
    half
    convert_to_half (
        const float            f32_value,
        const uint16_t * const base_table,
        const uint16_t * const shift_table,
        const uint16_t         table_length);
};


// MARK: == 函数实现 ==
INLINE_FUNCTION
uint32_t
HalfUtility::round_to_nearest_and_tie_to_even (
    const uint32_t value,
    const uint32_t shift)
{
    if (shift > 0)
    {
        const uint32_t half_way_value = (uint32_t)1 << (shift - 1);
        const uint32_t shifted_value  = value >> shift;
        const uint32_t truncated_part = value & (((uint32_t)1 << shift) - 1);

        if (truncated_part > half_way_value ||
            (truncated_part == half_way_value && ((shifted_value & 1) != 0)))
        {
            return shifted_value + 1;
        }
        else
        {
            return shifted_value;
        }
    }
    else
    {
        return value;
    }
}


INLINE_FUNCTION
half
HalfUtility::convert_to_half (
    const float            f32_value,
    const uint16_t * const base_table,
    const uint16_t * const shift_table,
    const uint16_t         table_length)
{
    const uint32_t f32_bits = MathUtility::bits_from_float32(f32_value);

    /// 处理特殊情况:
    /// +ZERO:  0x0000 0000
    /// +TINY: [0x0000 0001, 0x3300 0000]  ---> +ZERO(0x0000)
    if (f32_bits <= 0x33000000)
    {
        return half{ 0x0000 };
    }
    /// +TINY: [0x3300 0001, 0x337F FFFF]  ---> Minimal positive subnormal(0x0001)
    else if (f32_bits >= 0x33000001 && f32_bits <= 0x337FFFFF)
    {
        return half{ 0x0001 };
    }
    /// +HUGE: [0x4780 0000, 0x7F7F FFFF]  ---> +INF(0x7C00)
    /// +INF:   0x7F80 0000
    else if (f32_bits >= 0x47800000 && f32_bits <= 0x7F800000)
    {
        return half{ 0x7C00 };
    }
    /// +NAN: [0x7F80 0001, 0x7FFF FFFF]  ---> 0x7C01
    else if (f32_bits >= 0x7F800001 && f32_bits <= 0x7FFFFFFF)
    {
        return half{ 0x7C01 };
    }
    /// -ZERO:  0x8000 0000
    /// -TINY: [0x8000 0001, 0xB300 0000]  ---> -ZERO(0x8000)
    else if (f32_bits >= 0x80000000 && f32_bits <= 0xB3000000)
    {
        return half{ 0x8000 };
    }
    /// -TINY: [0xB300 0001, 0xB37F FFFF]  ---> Minimal negative subnormal(0x8001)
    else if (f32_bits >= 0xB3000001 && f32_bits <= 0xB37FFFFF)
    {
        return half{ 0x8001 };
    }
    /// -HUGE/: [0xC780 0000, 0xFF7F FFFF]  ---> -INF(0xFC00)
    /// -INF:    0xFF80 0000
    else if (f32_bits >= 0xC7800000 && f32_bits <= 0xFF800000)
    {
        return half{ 0xFC00 };
    }
    /// -NAN: [0xFF80 0001, 0xFFFF FFFF]  ---> 0xFC01
    else if (f32_bits >= 0xFF800001)
    {
        return half{ 0xFC01 };
    }
    else
    {
        /// HALF = Base[ Sign(FLOAT) | Exponent(FLOAT) ]
        ///      + RoundToNearAndTieToEven(
        ///         Mantissa(FLOAT), Shift[ Sign(FLOAT) | Exponent(FLOAT) ])
        ///
        // --- FLOAT(32位) ---//
        ///  31 (msb)
        ///  |
        ///  |  30     23
        ///  |  |      |
        ///  |  |      | 22                    0 (lsb)
        ///  |  |      | |                     |
        ///  X  XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        /// |S| |  E   | |         M           |
        ///
        /// 获得Sign以及Exponent的数值
        const uint32_t sign_exponent = (f32_bits >> 23) & 0x1FF; /// 9位
        if (sign_exponent < table_length)
        {
            // gets the base value and shift value
            const uint16_t base_value   = base_table[sign_exponent];
            const uint16_t shift_value  = shift_table[sign_exponent];
            const uint32_t mantissa_val = f32_bits & 0x7FFFFF; /// 23位
            const uint32_t rounded_bits =
                round_to_nearest_and_tie_to_even(mantissa_val, shift_value);

            return half{ (uint16_t)(base_value + rounded_bits) };
        }
        else
        {
            /// OUT OF BOUND: 返回 +NAN
            return half{ 0x7C01 };
        }
    }
}
