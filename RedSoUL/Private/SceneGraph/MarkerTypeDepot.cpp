/// Library headers
#include "Assert/RuntimeAssert.hpp"
/// Self header
#include "SceneGraph/MarkerTypeDepot.hpp"


MarkerTypeDepot &
MarkerTypeDepot::ref ()
{
    static MarkerTypeDepot s_instance;
    return s_instance;
}


void
MarkerTypeDepot::register_type (
    const StaticStringIdT  marker_name_id,
    const MarkerTypeInfo & type_info)
{
    const auto marker_info = m_type_table.find(marker_name_id);
    if (marker_info == m_type_table.end())
    {
        m_type_table.emplace(marker_name_id, &type_info);
    }
}


ObjectMarker *
MarkerTypeDepot::create_marker (
    SceneObject &         object_owner,
    const StaticStringIdT marker_name_id)
{
    auto stored_info = m_type_table.find(marker_name_id);
    RUNTIME_ASSERT(stored_info != m_type_table.end(),
                   "Please register this marker type by calling "
                   "register_type() at first!!");

    if (stored_info == m_type_table.end())
    {
        return nullptr;
    }
    else
    {
        return stored_info->second->create_function()(object_owner);
    }
}


bool
MarkerTypeDepot::destroy_marker (
    const StaticStringIdT marker_name_id,
    ObjectMarker * &      marker_object)
{
    auto stored_info = m_type_table.find(marker_name_id);
    RUNTIME_ASSERT(stored_info != m_type_table.end(),
                   "Please register this marker type by calling "
                   "register_type() at first!!");

    if (stored_info == m_type_table.end())
    {
        return false;
    }
    else
    {
        return stored_info->second->destroy_function()(marker_object);
    }
}
