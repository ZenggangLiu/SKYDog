/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Math/MathUtilities.hpp"
/// Self header
#include "DataType/HdrColor.hpp"


HdrColor
HdrColor::make (
    const float r,
    const float g,
    const float b,
    const float a)
{
    const float alpha = MathUtility::clamp(a, 0.0f, 1.0f);
    return HdrColor
    {
        half::make(r), half::make(g), half::make(b), half::make(alpha)
    };
}


HdrColor
HdrColor::make (
    const half r,
    const half g,
    const half b,
    const half a)
{
    const float alpha = MathUtility::clamp(a.to_float(), 0.0f, 1.0f);
    return HdrColor{ r, g, b, half::make(alpha) };
}


bool
HdrColor::operator== (
    const HdrColor color) const
{
    return MathUtility::equal(r.to_float(), color.r.to_float()) &&
           MathUtility::equal(g.to_float(), color.g.to_float()) &&
           MathUtility::equal(b.to_float(), color.b.to_float()) &&
           MathUtility::equal(a.to_float(), color.a.to_float());
}


bool
HdrColor::operator!= (
    const HdrColor color) const
{
    return !operator==(color);
}


half
HdrColor::operator[] (
    const uint8_t idx) const
{
    RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
    return e[idx];
}


half &
HdrColor::operator[] (
    const uint8_t idx)
{
    RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
    return e[idx];
}
