/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "SceneGraph/SceneObject.hpp"
/// Self header
#include "SceneGraph/Light/LightMarker.hpp"


LightType
LightMarker::light_type () const
{
    return m_light_type;
}


bool
LightMarker::cast_shadow () const
{
    return m_cast_shadow;
}


float_3
LightMarker::color () const
{
    return m_color;
}


float
LightMarker::intensity () const
{
    return m_intensity;
}


void
LightMarker::set_cast_shadow (
    const bool enabled)
{
    m_cast_shadow = enabled;
}


void
LightMarker::set_color (
    const float_3 color)
{
    RUNTIME_ASSERT(color.x >= 0.0f && color.y >= 0.0f && color.z >= 0.0f,
                   "Color value is not valid!!");
    m_color = color;
}


void
LightMarker::set_intensity (
    const float intensity)
{
    RUNTIME_ASSERT(intensity >= 0.0f, "Intensity value is not valid!!");
    m_intensity = intensity;
}


LightMarker::LightMarker (
    SceneObject &         marker_owner,
    const StaticStringIdT marker_name_id,
    const LightType       light_type,
    const float_3         light_color,
    const float           light_intensity,
    const bool            does_cast_shadow)
:
    SuperT(marker_owner, marker_name_id),
    m_transform_marker(marker_owner.transform()),
    m_color(light_color),
    m_intensity(light_intensity),
    m_cast_shadow(does_cast_shadow),
    m_light_type(light_type)
{

}


LightMarker::~LightMarker ()
{

}
