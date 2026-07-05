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
    Created:  05/06/26  @  10:52 PM
    FileName: CameraMarker.hpp @ RedSoUL Project
    History:
             - created by: 05/06/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "DataType/Float3D.hpp"
#include "DataType/Matrix3x4.hpp"
#include "DataType/Matrix4x4.hpp"
#include "SceneGraph/Camera/CameraType.hpp"
#include "SceneGraph/ObjectMarker.hpp"
#include "Text/StaticStringId.hpp"


class SceneObject;
class TransformMarker;


/// Camera属性基类
class CameraMarker : public ObjectMarker
{
public:
    /// 获取当前的照相机类型
    CameraType
    camera_type () const;

    /// 将相机空间的点变换到世界空间
    float_3
    camera_point_to_world_space (
        const float_3 camera_point) const;

    /// 将世界空间的点变换到相机空间
    float_3
    world_point_to_camera_space (
        const float_3 world_point) const;


protected:
    typedef ObjectMarker SuperT;

    CameraMarker (
        SceneObject &         marker_owner,
        const StaticStringIdT marker_name_id,
        const CameraType      camera_type);

    ~CameraMarker ();

    CameraMarker (
        const CameraMarker &) = delete;
    CameraMarker & operator = (
        const CameraMarker &) = delete;

    /// 获取相机变换矩阵: World --> Camera空间
    const matrix_3x4 &
    world_to_camera_transform () const;

    // === 事件处理 === //
    /// 世界变换更新
    void on_world_transform_updated ();

protected:
    /// 每种相机的起始个数
    static constexpr uint8_t INIT_CAMERA_COUNT = 2;

    /// 相机变换矩阵: 世界 --> 相机空间
    mutable matrix_3x4 m_camera_transform;
    /// 投影变换矩阵: 世界 --> 剪切空间
    mutable matrix_4x4 m_project_transform;
    /// 空间变换属性的参考
    TransformMarker &  m_transform_marker;
    /// 标记是否相机变换矩阵失效(World --> Camera)
    mutable bool       m_is_camera_transform_dirty;
    /// 标记是否投影变换矩阵失效(World --> Clip)
    mutable bool       m_is_projection_transform_dirty;
    /// 相机类型
    const CameraType   m_camera_type;
};
