/// System headers
#include <cstring> /// std::memcpy
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "DataType/Matrix3x4.hpp"
/// Self header
#include "DataType/Matrix4x4.hpp"


matrix_4x4
matrix_4x4::make (
    const matrix_3x4 & rts_matx)
{
    /// [ RS      | T
    ///   <0,0,0> | 1 ]
    matrix_4x4 ret_matx;

    /// 复制前三行
    std::memcpy(ret_matx.rows, rts_matx.rows, sizeof(matrix_3x4));
    ret_matx.rows[3] = float_4::make(0, 0, 0, 1);
    return ret_matx;
}


float_4
matrix_4x4::operator[] (
    const uint8_t idx) const
{
    RUNTIME_ASSERT(idx >= 0 && idx < ROW_COUNT, "Index is out of range");
    return rows[idx];
}


float_4 &
matrix_4x4::operator[] (
    const uint8_t idx)
{
    RUNTIME_ASSERT(idx >= 0 && idx < ROW_COUNT, "Index is out of range");
    return rows[idx];
}
