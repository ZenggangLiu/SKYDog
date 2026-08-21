/// Library headers
#include "DataType/HalfFloatConversionTable.hpp"
#include "DataType/HalfUtilities.hpp"
#include "Math/MathUtilities.hpp"
/// Self header
#include "DataType/Half.hpp"


half
half::make (
    const float f32_value)
{
    return HalfUtility::convert_to_half(
        f32_value,
        FLOAT_TO_HALF_BASE_TABLE, FLOAT_TO_HALF_SHIFT_TABLE,
        ARRAY_LENGTH(FLOAT_TO_HALF_BASE_TABLE));
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
    return MathUtility::float32_from_bits(f32_bits);
}
