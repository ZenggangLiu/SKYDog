/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Math/MathUtilities.hpp"
/// Self header
#include "DataType/Matrix3x4.hpp"


matrix_3x4
matrix_3x4::make ()
{
    return matrix_3x4
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0
    };
}


matrix_3x4
matrix_3x4::make (
    const float s)
{
    return matrix_3x4
    {
        s, 0, 0, 0,
        0, s, 0, 0,
        0, 0, s, 0
    };
}


matrix_3x4
matrix_3x4::make (
    const float_3 trans)
{
    const float x = trans.x;
    const float y = trans.y;
    const float z = trans.z;
    return matrix_3x4
    {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z
    };
}


bool
matrix_3x4::operator== (
    const matrix_3x4 & matx) const
{
    return rows[0] == matx.rows[0] &&
           rows[1] == matx.rows[1] &&
           rows[2] == matx.rows[2];
}


bool
matrix_3x4::operator!= (
    const matrix_3x4 & matx) const
{
    return !operator==(matx);
}


float_4
matrix_3x4::operator[] (
    const uint8_t idx) const
{
    RUNTIME_ASSERT(idx >= 0 && idx < ROW_COUNT, "Index is out of range");
    return rows[idx];
}


float_4 &
matrix_3x4::operator[] (
    const uint8_t idx)
{
    RUNTIME_ASSERT(idx >= 0 && idx < ROW_COUNT, "Index is out of range");
    return rows[idx];
}
