/// Library headers
#include "SceneGraph/MarkerTypeDepot.hpp"
/// Self header
#include "SceneGraph/ObjectMarker.hpp"


CreateParameter::CreateParameter(
    const StaticStringIdT _name_id,
    SceneObject &         _owner)
:
    owner(_owner),
    name_id(_name_id)
{

}



#if (BUILD_MODE == DEBUG_BUILD_MODE)
MarkerTypeInfo::MarkerTypeInfo (
    const char * const   _marker_name,
    const CreateFuncPtr  _create_func,
    const DestroyFuncPtr _destroy_func)
:
    create_func(_create_func),
    destroy_func(_destroy_func),
    marker_name(_marker_name)
{
    /// 注册此类型
    MarkerTypeDepot::ref().register_type(marker_name.id(), *this);
}


StaticStringIdT
MarkerTypeInfo::name_id() const
{
    return marker_name.id();
}


#else
MarkerTypeInfo::MarkerTypeInfo (
    const StaticStringIdT _name_id,
    const CreateFuncPtr   _create_func,
    const DestroyFuncPtr  _destroy_func)
:
    create_func(_create_func),
    destroy_func(_destroy_func),
    marker_name_id(_name_id)
{
    MarkerTypeDepot::ref().register_type(_name_id, *this);
}


StaticStringIdT
MarkerTypeInfo::name_id () const
{
    return marker_name_id;
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
