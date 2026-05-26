/// System headers
#include <algorithm> /// std::find
#include <new>
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION, UNUSED_VARIABLE
#include "Math/MathUtilities.hpp"   /// round_up_multiple_count
#include "Memory/BlockAllocator.hpp"
#include "SceneGraph/SceneObject.hpp"
#include "SceneGraph/TransformMarker.hpp"
/// Self header
#include "SceneGraph/GameScene.hpp"


/// 全局的唯一的当前(活跃的)关卡
static GameScene * g_active_scene = nullptr;


// MARK: == 场景物体分配器 ==
class SceneObjectAllocator
{
public:
    static
    INLINE_FUNCTION
    SceneObjectAllocator &
    ref ()
    {
        static SceneObjectAllocator s_instance;
        return s_instance;
    }

    INLINE_FUNCTION
    void *
    allocate ()
    {
        return m_allocator.allocate();
    }

    bool
    destroy(
        void * const alloc_addr)
    {
        return m_allocator.deallocate(alloc_addr);
    }


private:
    INLINE_FUNCTION
    SceneObjectAllocator ()
    {
        static constexpr uint32_t OBJECT_BYTE_SIZE = (uint32_t)sizeof(SceneObject);
        /// 起始的物体个数
        static constexpr uint16_t SCENEOBJECT_COUNT = 100;
        static constexpr uint32_t EXPECT_BYTE_SIZE  = OBJECT_BYTE_SIZE * SCENEOBJECT_COUNT;

        const bool is_initialized = m_allocator.initialize(
            MathUtility::round_up_multiple_count(EXPECT_BYTE_SIZE, BLOCK_ALLOCATOR_PAGE_SIZE), 2);
        UNUSED_VARIABLE(is_initialized);
        RUNTIME_ASSERT(is_initialized, "Can not initialize the allocator!!");
    }

    INLINE_FUNCTION
    ~SceneObjectAllocator ()
    {
        m_allocator.release();
    }

private:
    typedef BlockAllocator<SceneObject> AllocatorTypeT;

    AllocatorTypeT m_allocator;
};



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
    SceneObject * const scene_object =
        (SceneObject*)SceneObjectAllocator::ref().allocate();
    if (scene_object)
    {
        /// 构造SceneObject实例
        new(scene_object) SceneObject(this, father);
        /// 加入列表
        m_object_list.push_back(scene_object);
        if (father == nullptr)
        {
            m_toplevel_object_list.push_back(scene_object);
        }
    }

    return scene_object;
}


void
GameScene::destroy_object (
    SceneObject *& scene_object)
{
    if (scene_object)
    {
        SceneObjectListT::iterator object_it =
            std::find(m_object_list.begin(), m_object_list.end(), scene_object);
        /// 有效实例
        if (object_it != m_object_list.end())
        {
            /// 移除列表
            m_object_list.erase(object_it);
            /// 如果是TopLevel节点, 从TopLevel表中移除
            //if (scene_object->find_marker(TransformMarker)->)
            //{
            //    m_toplevel_object_list.erase(root_object_it);
            //}
            /// 调用析构函数
            scene_object->~SceneObject();
            /// 释放内存
            SceneObjectAllocator::ref().destroy(scene_object);
            /// 清除参考
            scene_object = nullptr;
        }
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
    while (m_object_list.size())
    {
        destroy_object(m_object_list.back());
    }
    m_object_list.clear();
    m_toplevel_object_list.clear();
}
