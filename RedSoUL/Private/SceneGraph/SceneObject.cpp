/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CompilerDefines.hpp" /// BUILD_MODE
#include "SceneGraph/MarkerTypeDepot.hpp"
#include "SceneGraph/Camera/OrthogonalCamera.hpp"
#include "SceneGraph/Camera/PerspectiveCamera.hpp"
#include "SceneGraph/TransformMarker.hpp"
#include "Text/StaticString.hpp"
/// Self header
#include "SceneGraph/SceneObject.hpp"


MessageId
SceneObject::next_marker_message_id ()
{
    static MessageId current_message_id{ 1 };

    return MessageId{ current_message_id.value++ };
}


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


OrthogonalCamera *
SceneObject::add_orthogonal_camera (
    const float view_width,
    const float aspect_ratio,
    const float near_plane_dist,
    const float far_plane_dist)
{
    OrthogonalCamera * const camera = add_marker(OrthogonalCamera);
    if (camera)
    {
        camera->set_view_volume_width(view_width);
        camera->set_aspect_ratio(aspect_ratio);
        camera->set_near_plane_distance(near_plane_dist);
        camera->set_far_plane_distance(far_plane_dist);
    }

    return camera;
}


PerspectiveCamera *
SceneObject::add_perspective_camera (
    const float fov_degrees,
    const float aspect_ratio,
    const float near_plane_dist)
{
    PerspectiveCamera * const camera = add_marker(PerspectiveCamera);
    if (camera)
    {
        camera->set_field_of_view(fov_degrees);
        camera->set_aspect_ratio(aspect_ratio);
        camera->set_near_plane_distance(near_plane_dist);
    }

    return camera;
}


void
SceneObject::register_message_observer (
    const MessageId       message_id,
    ObjectMarker * const  marker_objc,
    const MessageFunction mesasge_fuc)
{
    /// 创建Message Key
    const MessageKey msg_key(this, message_id);
    /// 查找是否有注册的监听器
    const MessageTableT::iterator observer_list = ms_message_table.find(msg_key);
    if (observer_list == ms_message_table.end())
    {
        MessageObserverInfoListT new_list;
        new_list.push_back(MessageObserverInfo(marker_objc, mesasge_fuc));
        ms_message_table.emplace(msg_key, new_list);
    }
    else
    {
        MessageObserverInfoListT & exist_list = observer_list->second;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        for (const auto observer : exist_list)
        {
            RUNTIME_ASSERT(observer.marker_objc  != marker_objc &&
                           observer.message_func != mesasge_fuc,
                           "Double messge observer registration!!");
        }
#endif
        exist_list.push_back(MessageObserverInfo(marker_objc, mesasge_fuc));
    }
}


void
SceneObject::trigger_message (
    const MessageId message_id)
{
    /// 创建Message Key
    const MessageKey msg_key(this, message_id);
    /// 查找是否有注册的监听器
    const MessageTableT::const_iterator observer_list = ms_message_table.find(msg_key);
    if (observer_list != ms_message_table.end())
    {
        for (auto & observer : observer_list->second)
        {
            observer.message_func(observer.marker_objc);
        }
    }
}


SceneObject::SceneObject (
    GameScene * const   owner_scene,
    SceneObject * const father_object)
:
    m_owner_scene(*owner_scene),
    m_self_name_id(StaticString::get_empty_string_id()),
    m_tag_name_id(StaticString::get_empty_string_id()),
    m_layer_id(0),
    m_is_enabled(true)
{
    TransformMarker * const transform = add_marker(TransformMarker);
    if (father_object)
    {
        father_object->transform().attach(*transform);
    }
}


SceneObject::~SceneObject ()
{
    /// 使用MarkerTypeDepot来销毁所有属性
    for (auto marker : m_marker_list)
    {
        MarkerTypeDepot::ref().destroy_marker(marker->m_marker_name_id, marker);
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
            if (marker->m_marker_name_id == name_id)
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
    SceneObject &         marker_owner,
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
            MarkerTypeDepot::ref().create_marker(marker_owner, marker_name_id);
        RUNTIME_ASSERT(marker, "Can not create new marker!!");

        if (marker)
        {
            m_marker_list.push_back(marker);
        }
        return marker;
    }
}
