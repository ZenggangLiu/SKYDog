/***************************************************************************************
                                                                                        
        *          .               *                              .               *     
        ███████╗██╗  ██╗██╗   ██╗        ██████╗  ██████╗  ██████╗         *            
        ██╔════╝██║ ██╔╝╚██╗ ██╔╝        ██╔══██╗██╔═══██╗██╔════╝                      
        ███████╗█████╔╝  ╚████╔╝         ██║  ██║██║   ██║██║  ███╗        .            
        ╚════██║██╔═██╗   ╚██╔╝          ██║  ██║██║   ██║██║   ██║                     
        ███████║██║  ██╗    ██║           ██████╔╝╚██████╔╝╚██████╔╝         *          
        ╚══════╝╚═╝  ╚═╝    ╚═╝           ╚═════╝  ╚═════╝  ╚═════╝                     
                                                                                        
        <~~~               .        SKY Dog Game                      ~~~>        *     
                                Real-Time | Cross-Platform           .                  
----------------------------------------------------------------------------------------
                                                                                        
                                  ,,                                                    
                  __           o-°°|\_____/)                                            
    Author:   (___()'`; Zee...  \_/|_)     )                                            
              /,    /`             \  __  /                                             
              \\"--\\              (_/ (_/                                              
    Created:  15/02/26  @  4:38 PM
    FileName: MathUtilities.hpp @ RedSoUL Project
    History:
             - created by: 15/02/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <algorithm> /// std::min, std::max
/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION
#include "Math/MathDefines.hpp"     /// EPSILON


/// MARK: - 函数定义
struct MathUtility
{
    /// 近似计算Arc-Cosine
    ///
    /// @param[in]  num
    ///     Cosine的数值
    /// @return
    ///     对应的角度(以弧度衡量), 角度值在[0, π]之间
    /// NOTE: 误差为大约0.001弧度
    static
    float
    fast_acos (
        const float num);

    /// Clamps给定的数值到给定的区间: [min, max]
    template < typename T >
    INLINE_FUNCTION
    static
    T
    clamp (
       const T val,
       const T min,
       const T max);

    /// 计算A相对于B四舍五入后的整数倍数(即：A/B)
    /// 例如：Math::multiple_of(-3, 2) == -1
    template < typename T >
    INLINE_FUNCTION
    static
    T
    multiple_of (
        const T a,
        const T b);

    /// 判断是否两个浮点数相等(近似相等)
    static
    bool
    equal (
        const float a,
        const float b,
        const float epsilon = EPSILON);
};


/// MARK: - 函数实现
template < typename T >
INLINE_FUNCTION
T
MathUtility::clamp (
    const T val,
    const T min,
    const T max)
{
    return std::min(std::max(val, min), max);
}


/// 整数类型的实现
template < typename T >
INLINE_FUNCTION
T
MathUtility::multiple_of (
    const T a,
    const T b)
{
    /// (a + b-1)/b = (a-1)/b + 1
    return (a - (T)1)/b + (T)1;
}

/// multiple_of()不可用于浮点数
template <>
INLINE_FUNCTION float MathUtility::multiple_of(const float, const float);
template <>
INLINE_FUNCTION double MathUtility::multiple_of(const double, const double);
