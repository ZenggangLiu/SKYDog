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
    const StaticStringIdT  name_id,
    const MarkerTypeInfo & type_info)
{
    const auto marker_info = m_type_table.find(name_id);
    if (marker_info == m_type_table.end())
    {
        m_type_table.emplace(name_id, &type_info);
    }
}


ObjectMarker *
MarkerTypeDepot::create_marker (
    const StaticStringIdT   name_id,
    const CreateParameter & parameter)
{
    auto stored_info = m_type_table.find(name_id);
    RUNTIME_ASSERT(stored_info != m_type_table.end(),
                   "Please register this marker type by calling "
                   "register_type() at first!!");

    if (stored_info == m_type_table.end())
    {
        return nullptr;
    }
    else
    {
        return stored_info->second->create_func(parameter);
    }
}


bool
MarkerTypeDepot::destroy_marker (
    const StaticStringIdT name_id,
    ObjectMarker * &      marker)
{
    auto stored_info = m_type_table.find(name_id);
    RUNTIME_ASSERT(stored_info != m_type_table.end(),
                   "Please register this marker type by calling "
                   "register_type() at first!!");

    if (stored_info == m_type_table.end())
    {
        return false;
    }
    else
    {
        return stored_info->second->destroy_func(marker);
    }
}
