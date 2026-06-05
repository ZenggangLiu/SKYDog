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
    Created:  16/05/26  @  6:50 PM
    FileName: SceneObject.hpp @ RedSoUL Project
    History:
             - created by: 16/05/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint16_t,...
#include <vector>
/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION, STRINGIFY
#include "Text/StaticStringId.hpp"


struct CreateParameter;
class  GameScene;
class  ObjectMarker;


/// 关卡中的物体
///
/// +--------------+
/// |  SceneObject |
/// +--------------+----------+
/// |        Transform        |
/// |         Marker          |
/// |     (Node Hierarchy)    |
/// +-------------------------+
/// |  Camera/Geometry/Light  |
/// |         Marker          |
/// +-------------------------+
/// |        Collider         |
/// |         Marker          |
/// +-------------------------+
/// |         ......          |
/// |         Marker          |
/// +-------------------------+
///
/// NOTE: 可以通过添加各种属性(ObjectMarker)来改变它的行为
///
class SceneObject
{
public:
    /// 获取指定属性
    ///
    /// @param[in]  type_name
    ///     属性类型的名称
    /// @return
    ///     第一个指定类型的属性实例, 如果此物体有此属性
    ///     nullptr,                如果此物体无此属性
    ///
    /// 使用方法:
    /// TransformMarker * const transform =
    ///     scene_object->find_marker(TransformMarker);
    #define find_marker(type_name) \
        /* NOTE: 使用Templated Function来提供CAST的支持 */ \
        find_marker_with_nameid_and_cast<type_name>( \
            STATIC_STRING_HASH(STRINGIFY(type_name)))

    /// 添加指定属性
    ///
    /// @param[in]  type_name
    ///     属性类型的名称
    /// @param[in]  create_param
    ///     创建属性使用的参数
    /// @return
    ///     新添加的属性实例, 如果成功
    ///     nullptr,         如果失败
    ///
    /// 使用方法:
    /// PinholeCamera * const camera =
    ///     scene_object->add_marker(PinholeCamera);
    #define add_marker(type_name, create_param) \
        /* NOTE: 使用Templated Function来提供CAST的支持 */ \
        add_marker_with_nameid_and_cast<type_name>( \
            STATIC_STRING_HASH(STRINGIFY(type_name)), create_param)

    /// 获取所属关卡
    const GameScene &
    owner_scene () const;

    GameScene &
    owner_scene ();


private:
    /// 构造一个物体
    /// NOTE: 只由GameScene::create_object()创建。可以确保owner始终有效。
    ///
    /// @param[in]   owner
    ///      所有者(关卡, 不可为nullptr)
    /// @param[in]   father
    ///      父节点(如果无, 设置为nullptr)
    SceneObject (
        GameScene * const   owner,
        SceneObject * const father);

    ~SceneObject ();

    SceneObject (
        const SceneObject &) = delete;
    SceneObject & operator = (
        const SceneObject &) = delete;

    template <typename MarkerType>
    INLINE_FUNCTION
    const MarkerType *
    find_marker_with_nameid_and_cast (
        const StaticStringIdT name_id) const
    {
        return (const MarkerType*)find_marker_with_nameid(name_id);
    }

    template <typename MarkerType>
    INLINE_FUNCTION
    MarkerType *
    find_marker_with_nameid_and_cast (
        const StaticStringIdT name_id)
    {
        return (MarkerType*)find_marker_with_nameid(name_id);
    }

    /// 获取指定类型的属性实例
    const ObjectMarker *
    find_marker_with_nameid (
        const StaticStringIdT name_id) const;

    ObjectMarker *
    find_marker_with_nameid (
        const StaticStringIdT name_id);

    template <typename MarkerType>
    INLINE_FUNCTION
    MarkerType *
    add_marker_with_nameid_and_cast (
        const StaticStringIdT   name_id,
        const CreateParameter & parameter)
    {
        return (MarkerType*)add_marker_with_nameid(name_id, parameter);
    }

    /// 添加指定类型的属性
    ObjectMarker *
    add_marker_with_nameid (
        const StaticStringIdT   name_id,
        const CreateParameter & parameter);

private:
    friend class GameScene;

    typedef std::vector<ObjectMarker*> MarkerListT;

    /// 所属关卡
    GameScene &     m_owner_scene;
    // === 属性列表(至少一个|24/32字节|8字节对齐) === //
    MarkerListT     m_marker_list;
    /// 物体的名称Id: 32位/64位(4/8字节)
    StaticStringIdT m_self_name_id;
    /// 物体的标记名称Id: 32位/64位(4/8字节)
    StaticStringIdT m_tag_name_id;
    /// 所在Layer Id(缺省都在0层)
    uint16_t        m_layer_id;

    // === FLAGS: 16位(2字节) === //
    /// 开启标记
    uint16_t        m_is_enabled : 1;
};
