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
    Created:  10/07/26  @  11:41 PM
    FileName: SpotLight.hpp @ RedSoUL Project
    History:
             - created by: 10/07/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "DataType/Float3D.hpp"
#include "SceneGraph/Light/RangedLight.hpp"


class SceneObject;


/// 聚光灯光源:
///
/// Eye👀__             🔦TORCH
///     |\    |N        *
///       \   |       * ωi    *
///     ωo \  |     *       * l
///         \ |   *     | *
///          \| *       +--
///  ---------P---------
///
/// 对于聚光灯TORCH, 我们认为它辐射出来的光能有角度衰减。
/// 即,
///          dΦ
/// I(ωi) = ──── = K * λ(ω) = s*C_rgb * λ(ω)
///          dω
///
/// λ(ω)为聚光灯的光强的分布函数
///
/// 对于观测点P, 沿着ωi方向输入的光亮度为: 只有单一方向l(ωl)上有光输入
/// Li(P, ωi) = Le(ωl) * δ(ωi - ωl) NOTE: 光源方向l(ωl)与ωi反向。即, 从BULB出发指向观测点P
///
/// 对于观测点P, 沿着ωo方向的输出光亮度为: NOTE: 下面所有方向都从P点出发, 并只有l(ωl)方向有光
///
///             ⌠
/// Lo(P, ωo) = ⎮ fr(P, ωi, ωo) * Li(P, ωi) * dot(ωi, N) * V(P, ωi) * dωi
///             ⌡
///             2π
///
///           = fr(P, -ωl, ωo) * E(P) * V(P, -ωl)
///
///                               I(ωl)
///           = fr(P, -ωl, ωo) * ─────── * λ(ωl) * dot(-ωl, N) * V(P, -ωl)
///                                d*d
///
///                               s*C_rgb
///           = fr(P, -ωl, ωo) * ───────── * λ(ωl) * dot(-ωl, N) * V(P, -ωl)
///                                 d*d
///
/// V(P, ωi)表示点P沿着ωi方向是否可以看到光源。
/// 与SUN, BULB的区别在于:
/// SUN:   Lo(P, ωo) =  s*C_rgb  * fr(p, -ωl, ωo) * dot(-ωl, N) * V(P, -ωl)
///
///                     s*C_rgb
/// BULB:  Lo(P, ωo) = ───────── * fr(P, -ωl, ωo) * dot(-ωl, N) * V(P, -ωl)
///                       d*d
///
///                     s*C_rgb
/// TORCH: Lo(P, ωo) = ───────── * λ(ωl) * fr(P, -ωl, ωo) * dot(-ωl, N) * V(P, -ωl)
///                       d*d
///
class Spotight : public RangedLight
{
public:
    float
    inner_falloff_angle () const;

    float
    outer_falloff_angle () const;

    /// 设定Falloff角度
    ///
    /// @param[in]  inner_angle_degs
    ///      起始衰减角度(以度来衡量)
    /// @param[in]  outer_angle_degs
    ///      终止衰减角度(以度来衡量)
    void
    set_falloff_angles (
        const float inner_angle_degs,
        const float outer_angle_degs);


protected:
    friend class SceneObject;

    typedef RangedLight SuperT;

    /// 创建一个缺省TORCH(白光, 作用域10米, Inner角21.8°, Outer角30°的TORCH)
    ///
    /// @param[in]  marker_owner
    ///     Marker的所有者
    static
    ObjectMarker *
    create (
        SceneObject & marker_owner);

    /// 销毁函数
    ///
    /// @param[in,out] marker_object
    ///     Marker实例。设置为nullptr, 如果销毁成功
    /// @return
    ///     True,  如果销毁成功
    ///     False, 如果销毁失败
    static
    bool
    destroy (
        ObjectMarker * & marker_object);

    Spotight (
        SceneObject & marker_owner,
        const float_3 light_color,
        const float   light_intensity,
        const float   inner_angle_degs,
        const float   outer_angle_degs,
        const float   light_range,
        const bool    does_cast_shadow);

    ~Spotight ();

    Spotight (
        const Spotight &) = delete;
    Spotight & operator = (
        const Spotight &) = delete;


private:
    /// 起始的点光源个数
    static constexpr uint16_t INIT_POINT_LIGHT_COUNT = 10;

    static const MarkerTypeInfo ms_type_info;

    /// Intensity分布函数 λ(ω):
    ///
    ///        |  (P - L) dot Forward - cosθ(outer_angle) |^2
    /// λ(ω) = | ──────────────────────────────────────── |
    ///        |   cosθ(inner_angle) - cosθ(outer_angle)  |
    ///
    /// 起始衰减角度(度)
    float m_inner_angle_degs;
    /// 终止衰减角度(度)
    float m_outer_angle_degs;

    // --- CACHED DATA --- //
    /// 起始衰减角度的cosine值
    float m_inner_angle_cos;
    /// 终止衰减角度的cosine值
    float m_outer_angle_cos;
};
