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
    Created:  10/07/26  @  11:23 PM
    FileName: SunLight.hpp @ RedSoUL Project
    History:
             - created by: 10/07/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "DataType/Float3D.hpp"
#include "SceneGraph/Light/LightMarker.hpp"


/// 阳光:
///
/// Eye👀__              ☀️ SUN
///     |\    |N        *
///       \   |       * ωi    *
///     ωo \  |     *       * l
///         \ |   *     | *
///          \| *       +--
///  ---------P---------
///
/// 对于SUN, 我们认为它的Le为常量。即, 不随着距离变化(与观测点P无关),
/// 并且只有单一方向l上有光能。
/// NOTE: SUN的方向l(ω)与ωi反向。即, 从SUN出发指向观测点P
///
/// SUN沿着方向l相对于观测点P的入射光亮度为:
/// Li(P, -ωl) = Le(ωl) = s*C_rgb * δ(ωi - ωl)
///
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
/// 观测点P沿着ωo方向的光亮度为: NOTE: 下面所有方向都是从点P出发的
///
///             ⌠
/// Lo(P, ωo) = ⎮ fr(P, ωi, ωo) * s*C_rgb*δ(ωi - (-ωl)) * dot(ωi, N) * V(P, ωi) * dωi
///             ⌡
///             2π
///
///             ⌠
///           = ⎮ fr(P, ωi, ωo) * s*C_rgb * dot(ωi, N) * V(P, ωi) * δ(ωi - (-ωl)) * dωi
///             ⌡
///             2π
/// 
///           = fr(P, -ωl, ωo) * s*C_rgb * dot(-ωl, N) * V(P, -ωl)
///
///           = s*C_rgb * fr(P, -ωl, ωo) * dot(-ωl, N) * V(P, -ωl)
///             光源属性   材质属性         几何属性
///
/// V(P, ωi)表示点P沿着ωi方向是否可以看到光源。
///
class SunLight : public LightMarker
{
public:
    /// 获取世界空间中的光的朝向l(从光源出发)
    float_3
    world_light_direction () const;

    /// 设置当前时间
    ///
    /// @param[in]  hour
    ///     几时。范围[0, 23]
    ///     NOTE: 0表示午夜12时
    /// @param[in]  minute
    ///     几分。范围[0, 59]
    void
    set_day_time (
        const uint8_t hour,
        const uint8_t minute);

protected:
    friend class SceneObject;

    typedef LightMarker SuperT;

    /// 创建一个缺省SUN(Greenwich, 1973年:10月:17日:10时:30分)
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

    SunLight (
        SceneObject & marker_owner);

    ~SunLight ();

    SunLight (
        const SunLight &) = delete;
    SunLight & operator = (
        const SunLight &) = delete;


private:
    /// 更新SUN的朝向
    void
    update_forward_vector (
        const uint8_t hour,
        const uint8_t minute);

    /// 更新: Le(ωl) := s*C_rgb * δ(ωi - ωl)
    void
    update_sun_Le();

private:
    static const MarkerTypeInfo ms_type_info;
};
