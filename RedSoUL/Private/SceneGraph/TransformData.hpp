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
    Created:  18/05/26  @  12:16 PM
    FileName: TransformData.hpp @ RedSoUL Project
    History:
             - created by: 18/05/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION
#include "DataType/Float3D.hpp"
#include "DataType/Quaternion.hpp"
#include "DataType/Matrix3x4.hpp"
#include "SceneGraph/TransformType.hpp"


/// 本地变换数据
class TransformData
{
public:
    /// 创建一个新的TransformData实例
    static
    TransformData *
    create ();

    /// 销毁当前TransformData实例
    bool
    destroy ();

    /// 获取当前位移
    INLINE_FUNCTION
    float_3
    position () const
    {
        return m_local_position;
    }

    /// 获取当前俯仰/Pitch(X轴)角度(以度来衡量)
    INLINE_FUNCTION
    float
    pitch () const
    {
        return m_local_rotation.x;
    }

    /// 获取当前偏航/Yaw(Y轴)角度(以度来衡量)
    INLINE_FUNCTION
    float
    yaw () const
    {
        return m_local_rotation.y;
    }

    /// 获取当前翻滚/Roll(Z轴)角度(以度来衡量)
    INLINE_FUNCTION
    float
    roll () const
    {
        return m_local_rotation.z;
    }

    /// 获取当前的Euler角(以度来衡量)
    INLINE_FUNCTION
    float_3
    euler_angles () const
    {
        return m_local_rotation;
    }

    /// 获取当前旋转
    /// NOTE:
    /// - 旋转顺序: Roll(Z轴), Pitch(X轴), Yaw(Y轴)
    quaternion
    rotation () const;

    /// 获取当前放缩
    INLINE_FUNCTION
    float_3
    scaling () const
    {
        return m_local_scaling;
    }

    /// 计算当前本地(Local --> Father)变换矩阵
    matrix_3x4
    local_transform (
        const TransformType transform_type) const;

    /// 设置位移
    void
    set_position (
        const float_3 position);

    /// 设置俯仰/Pitch(X轴)角度(以度来衡量)
    void
    set_pitch (
        const float angle_degs);

    /// 设置偏航/Yaw(Y轴)角度(以度来衡量)
    void
    set_yaw (
        const float angle_degs);

    /// 设置翻滚/Roll(Z轴)角度(以度来衡量)
    void
    set_roll (
        const float angle_degs);

    void
    set_rotation (
        const quaternion rotation);

    /// 设置放缩(NOTE: 不可设置相同放缩)
    void
    set_scaling (
        const float_3 scaling);

private:
    TransformData ();

    ~TransformData ();

    /// 设置Euler角
    void
    set_euler_angle (
        const float angle_degs,
        float &     stored_loc);

    /// 更新合成的旋转
    void
    update_combined_rotation () const;

private:
    /// 起始的变换个数
    static constexpr uint16_t INIT_TRANSFORM_COUNT = 100;

    // --- LOCAL TRANSFORM --- //
    float_3             m_local_position;
    /// 俯仰/Pitch(X轴), 偏航/Yaw(Y轴), 翻滚/Roll(Z轴)旋转角(以度来衡量):
    float_3             m_local_rotation; /// < Pitch(X轴), Yaw(Y轴), Roll(Z轴) >
    float_3             m_local_scaling;

    // --- CACHED DATA --- //
    /// 合成的本地旋转
    /// NOTE:
    /// - 合成顺序: 偏航(Y) * 俯仰(X) * 翻滚(Z)
    ///   如果旋转一个物体, 相当于先Yaw, 再Pitch, 最后Roll
    /// - 任意点P按照此公式变换: P' := 偏航*俯仰*翻滚 * P
    mutable quaternion  m_combined_rotation;

    // --- FLAGS --- //
    /// 标记是否合成的旋转数据失效
    mutable bool        m_is_combined_rotation_dirty;
};
