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

    /// 求A，B数值中的最大值
    template < typename T >
    INLINE_FUNCTION
    static
    T
    maximum (
       const T a,
       const T b);

    /// 求A，B数值中的最小值
    template < typename T >
    INLINE_FUNCTION
    static
    T
    minimum (
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
    return minimum(maximum(val, min), max);
}


template < typename T >
INLINE_FUNCTION
T
MathUtility::maximum (
    const T a,
    const T b)
{
    return a > b ? a : b;
}


template < typename T >
INLINE_FUNCTION
T
MathUtility::minimum (
    const T a,
    const T b)
{
    return a < b ? a : b;
}
