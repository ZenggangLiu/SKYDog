/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "SceneGraph/MarkerTypeDepot.hpp"
#include "SceneGraph/TransformMarker.hpp"
#include "Text/StaticString.hpp"
/// Self header
#include "SceneGraph/SceneObject.hpp"


const GameScene &
SceneObject::owner_scene () const
{
    return m_owner_scene;
}


GameScene &
SceneObject::owner_scene ()
{
    return const_cast<GameScene&>(((const SceneObject*)this)->owner_scene());
}


const TransformMarker &
SceneObject::transform () const
{
    const TransformMarker * const transform_marker = find_marker(TransformMarker);
    RUNTIME_ASSERT(transform_marker, "SceneObject has no TransformMarker!!");

    return *transform_marker;
}


TransformMarker &
SceneObject::transform ()
{
    return const_cast<TransformMarker&>(((const SceneObject*)this)->transform());
}


SceneObject::SceneObject (
    GameScene * const   owner,
    SceneObject * const father)
:
    m_owner_scene(*owner),
    m_self_name_id(StaticString::get_empty_string_id()),
    m_tag_name_id(StaticString::get_empty_string_id()),
    m_layer_id(0),
    m_is_enabled(true)
{
    TransformMarker * const transform = add_marker(TransformMarker);
    if (father)
    {
        father->transform().attach(*transform);
    }
}


SceneObject::~SceneObject ()
{
    /// 使用MarkerTypeDepot来销毁所有属性
    for (auto marker : m_marker_list)
    {
        MarkerTypeDepot::ref().destroy_marker(marker->m_name_id, marker);
    }
    m_marker_list.clear();
}


const ObjectMarker *
SceneObject::find_marker_with_nameid (
    const StaticStringIdT name_id) const
{
    if (name_id == INVALID_STATIC_STRING_ID)
    {
        return nullptr;
    }
    else
    {
        for (const auto marker : m_marker_list)
        {
            if (marker->m_name_id == name_id)
            {
                return marker;
            }
        }
        return nullptr;
    }
}


ObjectMarker *
SceneObject::find_marker_with_nameid (
    const StaticStringIdT name_id)
{
    return const_cast<ObjectMarker*>(
        ((const SceneObject*)this)->find_marker_with_nameid(name_id));
}


ObjectMarker *
SceneObject::add_marker_with_nameid (
    SceneObject &         object_owner,
    const StaticStringIdT marker_name_id)
{
    /// 添加TransformMarker时, 属性列表必须为空
    if (m_marker_list.size() &&
        marker_name_id == TransformMarker::ms_type_info.marker_name_id())
    {
        return nullptr;
    }
    else
    {
        ObjectMarker * const marker =
            MarkerTypeDepot::ref().create_marker(object_owner, marker_name_id);
        RUNTIME_ASSERT(marker, "Can not create new marker!!");

        if (marker)
        {
            m_marker_list.push_back(marker);
        }
        return marker;
    }
}
