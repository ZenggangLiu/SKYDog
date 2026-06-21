/// Library headers
#include "SceneGraph/MarkerTypeDepot.hpp"
/// Self header
#include "SceneGraph/ObjectMarker.hpp"


#if (BUILD_MODE == DEBUG_BUILD_MODE)
MarkerTypeInfo::MarkerTypeInfo (
    const char * const   marker_name,
    const CreateFuncPtr  create_func,
    const DestroyFuncPtr destroy_func)
:
    m_create_func(create_func),
    m_destroy_func(destroy_func),
    m_marker_name(marker_name)
{
    /// 注册此类型
    MarkerTypeDepot::ref().register_type(m_marker_name.id(), *this);
}
#else
MarkerTypeInfo::MarkerTypeInfo (
    const StaticStringIdT marker_name_id,
    const CreateFuncPtr   create_func,
    const DestroyFuncPtr  destroy_func)
:
    m_create_func(create_func),
    m_destroy_func(destroy_func),
    m_marker_name_id(marker_name_id)
{
    MarkerTypeDepot::ref().register_type(m_marker_name_id, *this);
}
#endif


CreateFuncPtr
MarkerTypeInfo::create_function () const
{
    return m_create_func;
}


DestroyFuncPtr
MarkerTypeInfo::destroy_function () const
{
    return m_destroy_func;
}


#if (BUILD_MODE == DEBUG_BUILD_MODE)
StaticStringIdT
MarkerTypeInfo::marker_name_id() const
{
    return m_marker_name.id();
}
#else
StaticStringIdT
MarkerTypeInfo::marker_name_id () const
{
    return m_marker_name_id;
}
#endif



ObjectMarker::ObjectMarker (
    const StaticStringIdT name_id,
    SceneObject &         owner)
:
    m_owner(owner),
    m_name_id(name_id)
{

}


ObjectMarker::~ObjectMarker ()
{

}
