/// Library headers
#include "DataType/HalfFloatConversionTable.hpp"
/// Self header
#include "DataType/Half.hpp"


half
half::make (
    const float value)
{
    const uint32_t f32_bits = *(const uint32_t*)&value;

    /// 处理特殊情况:
    /// - +NAN: [0x7F80 0001, 0x7F80 1FFF]  ---> 0x7C01
    /// - -NAN: [0xFF80 0001, 0xFF80 1FFF]  ---> 0xFC01
    if (f32_bits >= 0x7F800001 && f32_bits <= 0x7F801FFF)
    {
        return half{ 0x7C01 };
    }
    else if (f32_bits >= 0xFF800001 && f32_bits <= 0xFF801FFF)
    {
        return half{ 0xFC01 };
    }
    else
    {
        /// HALF = Base[ Sign(FLOAT) | Exponent(FLOAT) ]
        ///      | Mantissa(FLOAT) >> Shift[ Sign(FLOAT) | Exponent(FLOAT) ]
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
        const uint32_t sign_exp  = (f32_bits >> 23) & 0x1FF; /// 9位
        // gets the base value and shift value
        const uint16_t base_val  = FLOAT_TO_HALF_BASE_TABLE[sign_exp];
        const uint16_t shift_val = FLOAT_TO_HALF_SHIFT_TABLE[sign_exp];
        const uint32_t mantissa  = f32_bits & 0x7FFFFF; /// 23位
        return half{ (uint16_t)(base_val | (mantissa >> shift_val)) };
    }
}


float
half::to_float () const
{
    /// FLOAT = Base[ Sign(HALF) | Exponent(HALF) ]
    ///       | Mod[ Offset[Exponent(HALF)] + Mantissa(HALF) ]
    ///
    // --- HALF(16位) --- //
    ///  15 (msb)
    ///  |
    ///  |  14  10
    ///  |  |   |
    ///  |  |   | 9        0 (lsb)
    ///  |  |   | |        |
    ///  X  XXXXX XXXXXXXXXX
    /// |S| | E | |   M    |
    ///
    /// 获得Sign以及Exponent的数值
    const uint32_t sign_exp = (bits >> 10) & 0x3F; /// 6位
    const uint32_t base_val = HALF_TO_FLOAT_BASE_TABLE[sign_exp];
    const uint32_t offset   = HALF_TO_FLOAT_OFFSET_TABLE[sign_exp & 0x1F]; /// 5位
    const uint32_t mantissa = bits & 0x3FF; /// 10位
    const uint32_t mod_val  = HALF_TO_FLOAT_MOD_TABLE[offset + mantissa];
    const uint32_t f32_bits = base_val | mod_val;
    return *(const float*)&f32_bits;
}
