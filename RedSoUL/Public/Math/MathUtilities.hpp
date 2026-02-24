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


#include <algorithm> /// std::min, std::max
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION


/// MARK: - 函数定义
struct MathUtility
{
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
INLINE_FUNCTION Half MathUtility::multiple_of(const Half, const Half);
template <>
INLINE_FUNCTION Float MathUtility::multiple_of(const Float, const Float);
template <>
INLINE_FUNCTION Double MathUtility::multiple_of(const Double, const Double);
