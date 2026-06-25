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
    Created:  21/06/26  @  10:12 PM
    FileName: PerspectiveCamera.hpp @ RedSoUL Project
    History:
             - created by: 21/06/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "SceneGraph/Camera/CameraMarker.hpp"


/// 透视相机
/// NOTE:
/// - 使用Infinite Far Plane
/// - Reversed Depth/Z: 即, Near Plane映射到1, Far Plane映射到0
///
class PerspectiveCamera : public CameraMarker
{
public:
    /// 获取水平Fov(角度衡量)
    float
    field_of_view () const;

    /// 获取宽高比
    float
    aspect_ratio () const;

    /// 获取近平面距离(Camera空间, 沿Z轴)
    float
    near_plane_distance () const;

    /// 获取Clip空间变换矩阵: World --> Clip
    const matrix_4x4 &
    world_to_clip_space_transform () const;

    void
    set_field_of_view (
        const float fov_degress);

    void
    set_aspect_ratio (
        const float aspect_ratio);

    void
    set_near_plane_distance (
        const float distance);


private:
    friend class SceneObject;

    typedef CameraMarker SuperT;

    /// 创建一个缺省的透视相机: Fov(90°), 16比9, 10厘米(0.1米)Near Plane
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

    PerspectiveCamera (
        SceneObject & marker_owner,
        const float   fov_degrees,
        const float   aspect_ratio,
        const float   near_plane_dist);

    ~PerspectiveCamera ();

    PerspectiveCamera (
        const PerspectiveCamera &) = delete;
    PerspectiveCamera & operator = (
        const PerspectiveCamera &) = delete;

private:
    static const MarkerTypeInfo ms_type_info;

    /// 水平Fov(角度衡量)
    float m_field_of_view;
    /// 宽高比: 宽/高
    float m_aspect_ratio;
    /// 近平面距离(相机空间, 沿Z轴)
    float m_near_plane_distance;

    // --- CACHED DATA --- //
    /// cotangent(Fov * 0.5)
    float m_cotangent_half_fov;
};
