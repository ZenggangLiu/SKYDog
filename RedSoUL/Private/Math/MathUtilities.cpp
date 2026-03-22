/// System headers
#include <cmath> /// std::fabsf, std::sqrtf
/// Self header
#include "Math/MathUtilities.hpp"


float
MathUtility::fast_acos (
    const float num)
{
    /// 保证指定Cosine值在[-1, +1]之间
    const float clamped_cos = clamp(num, -1.f, +1.f);

    /// 获取Cosine值的符号。负数表示角度在[π/2, π]度之间
    const bool is_neg = clamped_cos < 0;

    /// 在第一象限中计算角度[0, π/2]
    const float positive_cos = std::fabsf(clamped_cos);
    float angle_rads = -0.0187293f;
    angle_rads = angle_rads * positive_cos + 0.0742610f;
    angle_rads = angle_rads * positive_cos - 0.2121144f;
    angle_rads = angle_rads * positive_cos + 1.5707288f;
    angle_rads = angle_rads * std::sqrtf(1.f - positive_cos);

    /// 如果在第二象限: π - angle
    return is_neg ? ONE_PI - angle_rads : angle_rads;
}


bool
MathUtility::equal (
    const float a,
    const float b,
    const float epsilon /* = EPSILON */)
{
    /// 参考：
    /// - Real Time Collision Detection
    /// 11.3.1：Tolerance Comparison for Floating-point Values
    ///
    /// 我们使用如下方式比较两个浮点数a, b:
    /// 1) 绝对tolerance比较: abs(a - b) <= tolerence
    ///    这种比较方式需要a与b中有一定的digit匹配，才能够认为他们相等。
    ///    但是当a与b越来越大的时候，这种比较几乎相当于比较所有a与b的digits是否相同
    /// 2) 相对tolerance比较:  abs(a/b - 1) <= tolerence, where |a| <= |b|
    ///    这个比较关系可以写为: abs(a - b) <= tolerence * abs(b)
    ///    如果忽略|a| <= |b|的条件：
    ///    + 我们可以获得：abs(a - b) <= tolerence * max(abs(a), abs(b))
    ///      这个比较公式只在abs(a)以及abs(b)都大于1的情况下成立
    ///
    /// 所以: absolute(a - b) <= tolerence * max[max(abs(a), abs(b)), 1]
    /// - 当数值小于1的时候，使用绝对比较: abs(a - b) <= tolerence * 1
    /// - 当数值大于1的时候，使用相对比较: abs(a - b) <= tolerence * max(abs(a), abs(b))
    /// NOTE:
    /// 使用如下近似无法实现1: 绝对tolerance比较
    /// absolute(a - b) <= tolerence * (absolute(a) + absolute(b) + 1)
    ///
    const float _max_a_b = std::max(std::fabsf(a), std::fabsf(b));
    return std::fabsf(a - b) <= epsilon * std::max(_max_a_b, 1.f);
}
