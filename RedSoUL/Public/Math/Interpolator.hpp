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
    Created:  10/07/26  @  9:41 PM
    FileName: Interpolator.hpp @ RedSoUL Project
    History:
             - created by: 10/07/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION
#include "Math/MathUtilities.hpp"


/// 插值器(对数值A, B插值): f(A, B, t), t ∈ [0,1]
/// f(A, B, t) := A + (B − A)*g(t)
/// g(t): Easing Function
/// NOTE: 不同的插值器使用不同的g(t)函数
///
struct Interpolator
{
    /// 线性插值: g(t) = t
    /// f(A, B, t) := A + (B − A)*t
    /// A -------- B
    /// 特性: 匀速运动
    ///
    /// @param[in]   start
    ///      起始点
    /// @param[in]   end
    ///      终止点
    /// @param[in]   factor
    ///      步进系数
    template < typename T >
    INLINE_FUNCTION
    static
    T
    linear (
        const T     start,   /// A
        const T     end,     /// B
        const float factor); /// t

    /// EaseIn: g(t) = t^2
    /// f(A, B, t) := A + (B − A)*t^2
    /// A --=====> B
    /// 特性: 加速运动
    ///
    /// @param[in]   start
    ///      起始点
    /// @param[in]   end
    ///      终止点
    /// @param[in]   factor
    ///      步进系数
    template < typename T >
    INLINE_FUNCTION
    static
    T
    ease_in (
        const T     start,   /// A
        const T     end,     /// B
        const float factor); /// t

    /// EaseOut: g(t) = 1 - (1-t)^2
    /// f(A, B, t) := A + (B − A)*[1 - (1-t)^2]
    ///            := A + (B − A)*t*(2-t)
    /// A =====--> B
    /// 特性: 减速运动
    ///
    /// @param[in]   start
    ///      起始点
    /// @param[in]   end
    ///      终止点
    /// @param[in]   factor
    ///      步进系数
    template < typename T >
    INLINE_FUNCTION
    static
    T
    ease_out (
        const T     start,   /// A
        const T     end,     /// B
        const float factor); /// t

    /// EaseInOut(Hermit曲线插值): g(t) = 3*t^2 - 2*t^3
    /// f(A, B, t) := A + (B − A)*(3*t^2 - 2*t^3)
    ///            := A + (B − A)*t*t*(3 - 2*t)
    /// A --====-- B
    /// 特性: 两端速度为零, 中间先加速再减速运动
    ///
    /// @param[in]   start
    ///      起始点
    /// @param[in]   end
    ///      终止点
    /// @param[in]   factor
    ///      步进系数
    template < typename T >
    INLINE_FUNCTION
    static
    T
    ease_in_out (
        const T     start,   /// A
        const T     end,     /// B
        const float factor); /// t

    template < typename T >
    INLINE_FUNCTION
    static
    T
    smooth (
        const T     start,   /// A
        const T     end,     /// B
        const float factor); /// t

    /// EaseInOut(Hermit曲线插值): g(t) = 6t^5 - 15t^4 + 10t^3
    /// f(A, B, t) := A + (B − A)*(6t^5 - 15t^4 + 10t^3)
    ///            := A + (B − A)*t*t*t*[t*(6*t - 15) + 10]
    /// A --====-- B
    /// 特性: 两端速度, 加速度为零, 中间先加速再减速运动
    /// - 在start点以及end点处的一阶/二阶导数为0
    template < typename T >
    INLINE_FUNCTION
    static
    T
    even_smoother (
        const T     start,   /// A
        const T     end,     /// B
        const float factor); /// t
};


// MARK: == 函数实现 ==
template < typename T >
INLINE_FUNCTION
T
Interpolator::linear (
    const T     start,  /// A
    const T     end,    /// B
    const float factor) /// t
{
    const float t = MathUtility::clamp(factor, 0.0f, 1.0f);

    /// f(A, B, t) := A + (B − A)*t
    return start + (end - start) * t;
}


template < typename T >
INLINE_FUNCTION
T
Interpolator::ease_in (
    const T     start,  /// A
    const T     end,    /// B
    const float factor) /// t
{
    const float t = MathUtility::clamp(factor, 0.0f, 1.0f);

    /// g(t) = t^2
    return linear(start, end, t * t);
}


template < typename T >
INLINE_FUNCTION
T
Interpolator::ease_out (
    const T     start,  /// A
    const T     end,    /// B
    const float factor) /// t
{
    const float t = MathUtility::clamp(factor, 0.0f, 1.0f);

    /// g(t) = 1 - (1-t)^2 = t*(2-t)
    return linear(start, end, t * (2.0f - t));
}


template < typename T >
INLINE_FUNCTION
T
Interpolator::ease_in_out (
    const T     start,  /// A
    const T     end,    /// B
    const float factor) /// t
{
    const float t = MathUtility::clamp(factor, 0.0f, 1.0f);

    /// g(t) = 3*t^2 - 2*t^3 = t*t*(3 - 2*t)
    return linear(start, end, t * t * (3.0f - 2.0f * t));
}


template < typename T >
INLINE_FUNCTION
T
Interpolator::smooth (
    const T     start,  /// A
    const T     end,    /// B
    const float factor) /// t
{
    return ease_in_out(start, end, factor);
}


template < typename T >
INLINE_FUNCTION
T
Interpolator::even_smoother (
    const T     start,  /// A
    const T     end,    /// B
    const float factor) /// t
{
    const float t = MathUtility::clamp(factor, 0.0f, 1.0f);

    /// g(t) = 6t^5 - 15t^4 + 10t^3 = t*t*t*[t*(6*t - 15) + 10]
    return linear(start, end, t * t * t * (t * (6.0f * t - 15.0f) + 10));
}
