/// System headers
#include <cmath> /// std::sqrtf
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Math/MathUtilities.hpp"
/// Self header
#include "DataType/Float4D.hpp"


const float_4 float_4::ZERO{ 0, 0, 0, 0 };
const float_4 float_4::ONE { 1, 1, 1, 1 };


float_4
float_4::make ()
{
    return ZERO;
}


float_4
float_4::make (
    const float x,
    const float y,
    const float z,
    const float w)
{
    return float_4{ x, y, z, w };
}


float_4
float_4::make_from_point (
    const float_3 pnt)
{
    return float_4{ pnt.x, pnt.y, pnt.z, 1 };
}


float_4
float_4::make_from_vector (
    const float_3 vec)
{
    return float_4{ vec.x, vec.y, vec.z, 0 };
}


bool
float_4::operator== (
    const float_4 vec) const
{
    return MathUtility::equal(x, vec.x) &&
           MathUtility::equal(y, vec.y) &&
           MathUtility::equal(z, vec.z) &&
           MathUtility::equal(w, vec.w);
}


bool
float_4::operator!= (
    const float_4 vec) const
{
    return !operator==(vec);
}


float
float_4::operator[] (
    const uint8_t idx) const
{
    RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
    return e[idx];
}


float &
float_4::operator[] (
    const uint8_t idx)
{
    RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
    return e[idx];
}


float
float_4::length () const
{
    return std::sqrtf(length_squared());
}


float
float_4::length_squared () const
{
    /// 计算Inner Product: Dot(THIS, THIS)
    return dot(*this);
}


float
float_4::dot (
    const float_4 vec) const
{
    return (x * vec.x) + (y * vec.y) + (z * vec.z) + (w * vec.w);
}


float_4
float_4::unified_vec () const
{
    float_4 normalized_vec = *this;
    normalized_vec.normalize();
    return normalized_vec;
}


void
float_4::normalize ()
{
    const float vec_length = length();

    /// 1/length
    /// NOTE: 如果向量长度为零, 1/length为零
    const float inv_length =
        MathUtility::equal(vec_length, 0) ? 0.0f : 1.0f / vec_length;
    x *= inv_length;
    y *= inv_length;
    z *= inv_length;
    w *= inv_length;
}
