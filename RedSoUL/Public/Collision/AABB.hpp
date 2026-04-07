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
    Created:  7/04/26  @  10:10 PM
    FileName: AABB.hpp @ RedSoUL Project
    History:
             - created by: 7/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "Collision/Ray3D.hpp"
#include "DataType/AxisType.hpp"
#include "DataType/Float3D.hpp"


/// AABB包围盒(Axis Aligned Bounding Box)
///
///              ^ Y
///              |     / Z
///        +-----|-------+ Pmax
///       /|     .   /  /|
///      / |        /  / |
///     +-------------+  |
///     |  |     |/   |  |
///     |  |     o----|.--------> X
///     |  |          |  |
///     |  +----------|- +
///     | /           | /
///     +-------------+
/// Pmin
///
struct AABB
{
    /// 最小点
    float_3 pmin;
    /// 最大点
    float_3 pmax;

    /// 计算当前包围盒的中间点
    float_3
    mid_point () const;

    /// 计算当前包围盒的最长的轴向
    AxisType
    longest_axis () const;

    /// 计算当前包围盒的对角线向量(从pmin指向pmax的向量)
    float_3
    diagonal_vec () const;

    /// 计算当前包围盒的六个面的面积总和
    float
    area () const;

    /// 计算指定射线当前包围盒相交点的Hit Time
    ///
    /// @param[in]  local_ray
    ///     检测用的射线(NOTE: 必须在当前包围盒的本地坐标系内)
    /// @return
    ///     相交处的T值
    ///     - 正数(或者零): 相交
    ///     - 负数: 不相交
    float
    calc_hit_time (
        const Ray3D local_ray) const;

    /// 设置当前包围盒为空
    void
    set_empty ();

    /// 将当前包围盒与另一个包围盒B合并
    void
    union_with (
        const AABB & box);
};
