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
    Created:  10/07/26  @  11:40 PM
    FileName: PointLight.hpp @ RedSoUL Project
    History:
             - created by: 10/07/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "DataType/Float3D.hpp"
#include "SceneGraph/Light/RangedLight.hpp"


class SceneObject;


/// 点光源:
///
/// Eye👀__             💡BULB
///     |\    |N        *
///       \   |       * ωi    *
///     ωo \  |     *       * l
///         \ |   *     | *
///          \| *       +--
///  ---------P---------
///
/// 对于点光源BULB, 我们认为它各个方向上辐射出来的光能相同。
/// 即,
///          dΦ
/// I(ωi) = ──── = K = s*C_rgb
///          dω
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
///             ⌠                           由于只有ωl方向有输入
///           = ⎮ fr(P, ωi, ωo) * Li(P, ωi)*δ(ωi - (-ωl)) * dot(ωi, N) * V(P, ωi) * dωi
///             ⌡
///             2π
///
///             ⌠
///           = ⎮ fr(P, ωi, ωo) * Li(P, ωi) * dot(ωi, N) * V(P, ωi) * δ(ωi - (-ωl)) * dωi
///             ⌡
///             2π
///
///           = fr(P, -ωl, ωo) * Li(P, -ωl) * dot(-ωl, N) * V(P, -ωl)
///
///           = fr(P, -ωl, ωo) * E(P) * V(P, -ωl) <-- E(P) = Li(P, -ωl) * dot(-ωl, N)
///
///                               I(ωl)  <-- E(P) = I(ωl)/d^2 * dot(-ωl, N)
///           = fr(P, -ωl, ωo) * ─────── * dot(-ωl, N) * V(P, -ωl)
///                                d*d
///
///                               s*C_rgb
///           = fr(P, -ωl, ωo) * ───────── * dot(-ωl, N) * V(P, -ωl) <-- Intensity不随传播改变
///                                 d*d
///
///              s*C_rgb
///           = ───────── * fr(P, -ωl, ωo) * dot(-ωl, N) * V(P, -ωl)
///                d*d
///             光源属性     材质属性         几何属性
///
/// V(P, ωi)表示点P沿着ωi方向是否可以看到光源。
/// 与SUN的区别在于:
/// SUN:  Lo(P, ωo) =  s*C_rgb  * fr(p, -ωl, ωo) * dot(-ωl, N) * V(P, -ωl)
///
///                    s*C_rgb
/// BULB: Lo(P, ωo) = ───────── * fr(P, -ωl, ωo) * dot(-ωl, N) * V(P, -ωl)
///                      d*d
///
/// 由于:
/// 1. 点P处的光密度(Irradiance) E为: 对上半球积分
///          dΦ      ⌠
/// E(P) = ─────── = ⎮ Li(P, ωi) * dot(ωi, N) * dωi
///         dA(P)    ⌡
///                  2π
///
///        ⌠
///      = ⎮ Li(P, ωi)*δ(ωi - (-ωl)) * dot(ωi, N) * dωi
///        ⌡
///        2π
///
///        ⌠
///      = ⎮ Li(P, ωi) * dot(ωi, N) * δ(ωi - (-ωl)) * dωi
///        ⌡
///        2π
///
///      = Li(P, -ωl) * dot(-ωl, N)
///
/// dA(P)为点P处的微平面
///
/// 2. 光源的Itensity I(ωi)沿着ωl方向入射到达点P处的光强度为:
///          dΦ       dΦ                  dΦ
/// I(ωl) = ───── = ───── * d*d = ────────────────── * d*d
///          dωl     dA⟂          dA(P)*dot(-ωl, N)
/// dA⟂(垂直的微平面)与dωl的关系:
///         dA⟂
/// dωl = ───────
///         d*d
///
/// 3. 可以得到:
///           dΦ            1                           1
/// I(ωl) = ─────── * ───────────── * d*d = E(P) * ──────────── * d*d
///          dA(P)     dot(-ωl, N)                 dot(-ωl, N)
///
/// 最终得到:
///              I(ωl)
///  ==> E(P) = ─────── * dot(-ωl, N)
///               d*d
///
/// 4. Delta函数
/// δ(ωi - ωl)为Dirac Delta函数, 表示ωi只在ωl处有数值(+∞​)
///             ⎧ ∞,  x = xa
/// δ(x - xa) = ⎨
///             ⎩ 0,  x ≠ xa
///
///         +∞
///         ⌠
/// f(xa) = ⎮ f(x) * δ(x - xa) * dx
///         ⌡
///         -∞
///
class PointLight : public RangedLight
{
protected:
    friend class SceneObject;

    typedef RangedLight SuperT;

    /// 创建一个缺省BULB(白光, 作用域10米的BULB)
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

    PointLight (
        SceneObject & marker_owner,
        const float_3 light_color,
        const float   light_intensity,
        const float   light_range,
        const bool    does_cast_shadow);

    ~PointLight ();

    PointLight (
        const PointLight &) = delete;
    PointLight & operator = (
        const PointLight &) = delete;


private:
    /// 起始的点光源个数
    static constexpr uint16_t INIT_POINT_LIGHT_COUNT = 10;

    static const MarkerTypeInfo ms_type_info;
};
