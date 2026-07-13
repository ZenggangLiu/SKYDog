/// System headers
#include <algorithm> /// std::find
#include <new>
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CompilerDefines.hpp" /// BUILD_MODE
#include "Memory/BlockAllocator.hpp"
#include "SceneGraph/SceneObject.hpp"
#include "SceneGraph/TransformMarker.hpp"
/// Self header
#include "SceneGraph/GameScene.hpp"


/// 全局的唯一的当前(活跃的)关卡
static GameScene * g_active_scene = nullptr;


// MARK: == 游戏场景 ==
GameScene *
GameScene::active_scene ()
{
    return g_active_scene;
}


GameScene *
GameScene::create_empty_scene ()
{
    GameScene * const empty_scene = new (std::nothrow) GameScene();
    if (empty_scene)
    {
        /// 如果有Active Scene, 先销毁它
        if (g_active_scene)
        {
            delete g_active_scene;
        }

        g_active_scene = empty_scene;
    }
    return empty_scene;
}


SceneObject *
GameScene::create_object (
    SceneObject * const father)
{
    /// 申请内存
    void  * const new_object =
        BlockAllocator<SceneObject, INIT_SCENEOBJECT_COUNT>::ref().allocate();
    if (new_object)
    {
        /// 构建实例
        new(new_object) SceneObject(this, father);
        /// 加入列表
        m_object_list.push_back((SceneObject*)new_object);
    }

    return (SceneObject*)new_object;
}


void
GameScene::destroy_object (
    SceneObject *& scene_object)
{
    if (scene_object)
    {
        const SceneObjectListT::iterator object_it =
            std::find(m_object_list.begin(), m_object_list.end(), scene_object);
        /// 有效实例
        if (object_it != m_object_list.end())
        {
            /// 移除列表
            m_object_list.erase(object_it);
            /// 调用析构函数
            scene_object->~SceneObject();
            /// 释放内存
            BlockAllocator<SceneObject, INIT_SCENEOBJECT_COUNT>::ref().deallocate(
                scene_object);
            /// 清除参考
            scene_object = nullptr;
        }
    }
}


void
GameScene::register_toplevel_object (
    SceneObject & scene_object)
{
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    const SceneObjectListT::iterator toplevel_object_it =
        std::find(m_toplevel_object_list.begin(),
            m_toplevel_object_list.end(),
            &scene_object);
    RUNTIME_ASSERT(toplevel_object_it == m_toplevel_object_list.end(),
                   "Same scene object can not be inserted to "
                   "the toplevel list more than once!!");
#endif
    m_toplevel_object_list.push_back(&scene_object);
}


void
GameScene::unregister_toplevel_object (
    SceneObject & scene_object)
{
    const SceneObjectListT::iterator toplevel_object_it =
        std::find(m_toplevel_object_list.begin(),
                  m_toplevel_object_list.end(),
                  &scene_object);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    RUNTIME_ASSERT(toplevel_object_it != m_toplevel_object_list.end(),
                   "The given scene object is not a toplevel object!!");
#endif

    if (toplevel_object_it != m_toplevel_object_list.end())
    {
        m_toplevel_object_list.erase(toplevel_object_it);
    }
}


GameScene::GameScene ()
{

}


GameScene::~GameScene ()
{
    destroy_all_objects();

    /// 移除全局参考
    if (g_active_scene == this)
    {
        g_active_scene = nullptr;
    }
}


void
GameScene::destroy_all_objects ()
{
    /// 从列表尾至头依次销毁每个SceneObject: 使用创建时相反的顺序
    while (m_object_list.size() > 0)
    {
        destroy_object(m_object_list.back());
    }
    m_object_list.clear();
    m_toplevel_object_list.clear();
}
