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
    Created:  24/06/26  @  10:22 PM
    FileName: OrthogonalCamera.hpp @ RedSoUL Project
    History:
             - created by: 24/06/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "SceneGraph/Camera/CameraMarker.hpp"


/// 正交相机
/// NOTE:
/// - Reversed Depth/Z: 即, Near Plane映射到1, Far Plane映射到0
///
class OrthogonalCamera : public CameraMarker
{
public:
    /// 获取可视区域的宽度
    float
    view_volume_width () const;

    /// 获取可视区域的高度
    float
    view_volume_height () const;

    /// 获取近平面距离(相机空间, 沿Z轴)
    float
    near_plane_distance () const;

    /// 获取远平面距离(相机空间, 沿Z轴)
    float
    far_plane_distance () const;

    /// 获取Clip空间变换矩阵: World --> Clip空间
    const matrix_4x4 &
    world_to_clip_space_transform () const;

    void
    set_view_volume_width (
        const float width);

    void
    set_aspect_ratio (
        const float aspect_ratio);

    void
    set_near_plane_distance (
        const float distance);

    void
    set_far_plane_distance (
        const float distance);


private:
    friend class SceneObject;

    typedef CameraMarker SuperT;

    /// 创建一个缺省的正交相机: 可视宽带5米, 16比9, 10厘米(0.1米)Near Plane, 1000米Far Plane
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

    OrthogonalCamera (
        SceneObject & marker_owner,
        const float   view_width,
        const float   aspect_ratio,
        const float   near_plane_dist,
        const float   far_plane_dist);

    ~OrthogonalCamera ();

    OrthogonalCamera (
        const OrthogonalCamera &) = delete;
    OrthogonalCamera & operator = (
        const OrthogonalCamera &) = delete;

private:
    static const MarkerTypeInfo ms_type_info;

    /// 可视长方体的宽度(相机空间, 沿X轴)
    float m_view_volume_width;
    /// 宽高比: 宽/高
    float m_aspect_ratio;
    /// 近平面距离(相机空间, 沿Z轴)
    float m_near_plane_distance;
    /// 远平面距离(相机空间, 沿Z轴)
    float m_far_plane_distance;

    // === CACHED DATA === //
    /// 计算的可视长方体的高度(相机空间, 沿Y轴)
    float m_view_volume_height;
    /// 2 * 1/width
    float m_inverse_view_width_x2;
    /// 2 * 1/height
    float m_inverse_view_height_x2;
    /// 1/(far - near)
    float m_inverse_distance_delta;
};
