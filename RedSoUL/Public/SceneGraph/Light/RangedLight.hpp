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
    Created:  10/07/26  @  11:39 PM
    FileName: RangedLight.hpp @ RedSoUL Project
    History:
             - created by: 10/07/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "SceneGraph/Light/LightMarker.hpp"
#include "SceneGraph/Light/LightType.hpp"
#include "Text/StaticStringId.hpp"


class SceneObject;


/// 区域光源(带有基于点P到光源L距离d的衰减函数A(d)的光源):
///
/// Eye👀__             💡L
///     |\    |N        *
///       \   |       * d
///     ωo \  |     *
///         \ |   *
///          \| *
///  ---------P---------
///
class RangedLight : public LightMarker
{
public:
    /// 获取衰减距离(米)
    float
    range () const;

    /// 设置衰减距离(米)
    void
    set_range (
        const float range);


protected:
    friend class SceneObject;

    typedef LightMarker SuperT;

    RangedLight (
        SceneObject &         marker_owner,
        const StaticStringIdT marker_name_id,
        const LightType       light_type,
        const float_3         light_color,
        const float           light_intensity,
        const float           light_range,
        const bool            does_cast_shadow);

    ~RangedLight ();

    RangedLight (
        const RangedLight &) = delete;
    RangedLight & operator = (
        const RangedLight &) = delete;


protected:
    /// 衰减函数:
    ///
    ///                d*d
    /// A(d) := [ 1 - ───── ]^2
    ///                R*R
    /// - R为Range(衰减距离)
    /// - d*d为点P到光源L的距离的平方: d*d = (L - P) dot (L - P)
    ///
    /// 衰减距离(米)
    float m_range;
};
