/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Math/MathUtilities.hpp"
/// Self header
#include "DataType/LdrColor.hpp"


LdrColor
LdrColor::make (
    const uint8_t r,
    const uint8_t g,
    const uint8_t b,
    const uint8_t a)
{
    return LdrColor{ r, g, b, a };
}


bool
LdrColor::operator== (
    const LdrColor color) const
{
    return MathUtility::equal(r, color.r) &&
           MathUtility::equal(g, color.g) &&
           MathUtility::equal(b, color.b) &&
           MathUtility::equal(a, color.a);
}


bool
LdrColor::operator!= (
    const LdrColor color) const
{
    return !operator==(color);
}


uint8_t
LdrColor::operator[] (
    const uint8_t idx) const
{
    RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
    return e[idx];
}


uint8_t &
LdrColor::operator[] (
    const uint8_t idx)
{
    RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
    return e[idx];
}
