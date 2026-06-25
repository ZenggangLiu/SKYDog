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
#include <unordered_map>
#include <vector>
/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION, STRINGIFY
#include "Message/MessageDefines.hpp"
#include "Text/StaticStringId.hpp"


class GameScene;
class ObjectMarker;
class OrthogonalCamera;
class PerspectiveCamera;
class TransformMarker;


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
    /// Marker消息处理函数
    ///
    /// @param[in]  marker_object
    ///     作为监听器的Marker实例
    typedef void (*MessageFunction)(ObjectMarker * const marker_object);

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
    /// @return
    ///     新添加的属性实例, 如果成功
    ///     nullptr,         如果失败
    ///
    /// 使用方法:
    /// PerspectiveCamera * const camera = scene_object->add_marker(PerspectiveCamera);
    #define add_marker(type_name) \
        /* NOTE: 使用Templated Function来提供CAST的支持 */ \
        add_marker_with_nameid_and_cast<type_name>( \
            STATIC_STRING_HASH(STRINGIFY(type_name)))

    /// 获取下一个可用的消息Id
    static
    MessageId
    next_marker_message_id ();

    /// 获取所属关卡
    const GameScene &
    owner_scene () const;

    GameScene &
    owner_scene ();

    /// 获取空间变换属性
    const TransformMarker &
    transform () const;

    TransformMarker &
    transform ();

    /// 添加一个正交相机
    OrthogonalCamera *
    add_orthogonal_camera (
        const float view_width,
        const float aspect_ratio,
        const float near_plane_dist,
        const float far_plane_dist);

    /// 添加一个透视相机
    PerspectiveCamera *
    add_perspective_camera (
        const float fov_degrees,
        const float aspect_ratio,
        const float near_plane_dist);

    /// 注册一个Marker消息监听器
    ///
    /// @param[in]  message_id
    ///     希望监听的消息Id
    /// @param[in]  marker_objc
    ///     监听器(其它Marker)实例
    /// @param[in]  mesasge_func
    ///     消息处理函数
    void
    register_message_observer (
        const MessageId       message_id,
        ObjectMarker * const  marker_objc,
        const MessageFunction mesasge_func);

    /// 触发Marker消息
    void
    trigger_message (
        const MessageId message_id);


private:
    /// 构造一个物体
    /// NOTE: 只由GameScene::create_object()创建。可以确保owner_scene始终有效
    ///
    /// @param[in]   owner_scene
    ///      所有者(关卡, 不可为nullptr)
    /// @param[in]   father_object
    ///      父节点(如果无, 设置为nullptr)
    SceneObject (
        GameScene * const   owner_scene,
        SceneObject * const father_object);

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
        const StaticStringIdT marker_name_id)
    {
        return (MarkerType*)add_marker_with_nameid(*this, marker_name_id);
    }

    /// 添加指定类型的属性
    ///
    /// @param[in]  marker_owner
    ///     Marker的所有者
    /// @param[in]  marker_name_id
    ///     Marker类型名称的Id
    ObjectMarker *
    add_marker_with_nameid (
        SceneObject &         marker_owner,
        const StaticStringIdT marker_name_id);

private:
    friend class GameScene;

    struct MessageKey
    {
        SceneObject * const scene_objc;
        const MessageId     message_id;

        INLINE_FUNCTION
        MessageKey (
            SceneObject * const _scene_objc,
            const MessageId     _message_id)
        :
            scene_objc(_scene_objc),
            message_id(_message_id)
        {

        }
    };

    struct MessageKeyHasher
    {
        INLINE_FUNCTION
        std::size_t
        operator() (
            const MessageKey message_key) const
        {
            /// 计算Hash时使用的32位Seed
            static constexpr uint32_t ID_HASH_SEED = FOUR_CC_32('M', 'K', 'E', 'Y');

            return RUNTIME_HASH_64(ID_HASH_SEED, &message_key, sizeof(message_key));
        }
    };

    struct MessageKeyComparer
    {
        /// Compare: if A == B
        INLINE_FUNCTION
        bool
        operator() (
            const MessageKey key_a,
            const MessageKey key_b) const
        {
            return (key_a.scene_objc == key_b.scene_objc) &&
                   (key_a.message_id == key_b.message_id);
        }
    };

    struct MessageObserverInfo
    {
        ObjectMarker * const  marker_objc;
        MessageFunction const message_func;

        INLINE_FUNCTION
        MessageObserverInfo (
            ObjectMarker * const  _marker_objc,
            MessageFunction const _message_func)
        :
            marker_objc(_marker_objc),
            message_func(_message_func)
        {

        }
    };

    typedef std::vector< ObjectMarker* > MarkerListT;
    typedef std::vector< MessageObserverInfo > MessageObserverInfoListT;
    typedef std::unordered_map< MessageKey,
                                MessageObserverInfoListT,
                                MessageKeyHasher,
                                MessageKeyComparer > MessageTableT;

    static
    MessageTableT   ms_message_table;

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
