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
    const float_3 vec)
{
    const float x = vec.x;
    const float y = vec.y;
    const float z = vec.z;
    return matrix_3x4
    {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z
    };
}


matrix_3x4
matrix_3x4::make (
    const quaternion quat)
{
    /// 旋转矩阵为:
    ///      | 1 - 2*y*y - 2*z*z   2*x*y - 2*z*w       2*x*z + 2*y*w     |
    ///  R = | 2*x*y + 2*z*w       1 - 2*x*x - 2*z*z   2*y*z - 2*x*w     |
    ///      | 2*x*z - 2*y*w       2*y*z + 2*x*w       1 - 2*x*x - 2*y*y |
    ///
    const float xx2 = 2 * quat.x * quat.x;
    const float yy2 = 2 * quat.y * quat.y;
    const float zz2 = 2 * quat.z * quat.z;
    const float xy2 = 2 * quat.x * quat.y;
    const float xz2 = 2 * quat.x * quat.z;
    const float xw2 = 2 * quat.x * quat.w;
    const float yz2 = 2 * quat.y * quat.z;
    const float yw2 = 2 * quat.y * quat.w;
    const float zw2 = 2 * quat.z * quat.w;

    return matrix_3x4
    {
        1 - yy2 - zz2,  xy2 - zw2,      xz2 + yw2,
        xy2 + zw2,      1 - xx2 - zz2,  yz2 - xw2,
        xz2 - yw2,      yz2 + xw2,      1 - xx2 - yy2
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


matrix_3x4
matrix_3x4::operator* (
    const matrix_3x4 & matx) const
{
    ///         THIS           *          M
    /// | e00  e01  e02  e03 |   | e00  e01  e02  e03 |
    /// | e10  e11  e12  e13 | * | e10  e11  e12  e13 |
    /// | e20  e21  e22  e23 |   | e20  e21  e22  e23 |
    /// |  0    0    0    1  |   |  0    0    0    1  |
    ///
    const float_4 mx_col0 = float_4::make(matx.e00, matx.e10, matx.e20, 0.0f);
    const float_4 mx_col1 = float_4::make(matx.e01, matx.e11, matx.e21, 0.0f);
    const float_4 mx_col2 = float_4::make(matx.e02, matx.e12, matx.e22, 0.0f);
    const float_4 mx_col3 = float_4::make(matx.e03, matx.e13, matx.e23, 1.0f);

    const float combined_e00 = rows[0].dot(mx_col0);
    const float combined_e01 = rows[0].dot(mx_col1);
    const float combined_e02 = rows[0].dot(mx_col2);
    const float combined_e03 = rows[0].dot(mx_col3);

    const float combined_e10 = rows[1].dot(mx_col0);
    const float combined_e11 = rows[1].dot(mx_col1);
    const float combined_e12 = rows[1].dot(mx_col2);
    const float combined_e13 = rows[1].dot(mx_col3);

    const float combined_e20 = rows[2].dot(mx_col0);
    const float combined_e21 = rows[2].dot(mx_col1);
    const float combined_e22 = rows[2].dot(mx_col2);
    const float combined_e23 = rows[2].dot(mx_col3);

    return matrix_3x4
    {
        combined_e00,  combined_e01,  combined_e02,  combined_e03,
        combined_e10,  combined_e11,  combined_e12,  combined_e13,
        combined_e20,  combined_e21,  combined_e22,  combined_e23
    };
}


float_3
matrix_3x4::transform_point (
    const float_3 pnt) const
{
    ///         THIS           *    P
    /// | e00  e01  e02  e03 |   | P.x |
    /// | e10  e11  e12  e13 | * | P.y |
    /// | e20  e21  e22  e23 |   | P.z |
    /// |  0    0    0    1  |   |  1  |
    ///
    /// 构建齐次坐标
    const float_4 pnt_4d = float_4::make(pnt.x, pnt.y, pnt.z, 1.0f);

    return float_3::make(
        rows[0].dot(pnt_4d), rows[1].dot(pnt_4d), rows[2].dot(pnt_4d));
}
