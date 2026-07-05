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
    Created:  5/07/26  @  9:53 PM
    FileName: LightMarker.hpp @ RedSoUL Project
    History:
             - created by: 5/07/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "DataType/Float3D.hpp"
#include "SceneGraph/Light/LightType.hpp"
#include "SceneGraph/ObjectMarker.hpp"


class SceneObject;
class TransformMarker;


class LightMarker : public ObjectMarker
{
public:
    /// 获取光源类型
    LightType
    light_type () const;

    /// 检测光源是否产生阴影
    bool
    cast_shadow () const;

    float_3
    color () const;

    float
    intensity () const;

    void
    set_cast_shadow (
        const bool enabled);

    void
    set_color (
        const float_3 color);

    void
    set_intensity (
        const float intensity);


protected:
    typedef ObjectMarker SuperT;

    LightMarker (
        SceneObject &         marker_owner,
        const StaticStringIdT marker_name_id,
        const LightType       light_type);

    ~LightMarker ();

    LightMarker (
        const LightMarker &) = delete;
    LightMarker & operator = (
        const LightMarker &) = delete;

protected:
    /// 空间变换属性的参考
    TransformMarker & m_transform_marker;
    float_3           m_color;
    float             m_intensity;
    bool              m_cast_shadow;
    const LightType   m_light_type;
};
