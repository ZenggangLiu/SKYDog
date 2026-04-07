/// Self header
#include "Collision/Ray3D.hpp"


Ray3D
Ray3D::make (
    const float_3 start,
    const float_3 target)
{
    /// 计算从Start到End点的向量
    const float_3 direction = target - start;
    return Ray3D{ start, direction.unified_vec() };
}


float_3
Ray3D::calc_point (
    const float time) const
{
    return org + dir * time;
}
