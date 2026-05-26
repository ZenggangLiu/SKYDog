/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION, UNUSED_VARIABLE
#include "Math/MathUtilities.hpp"   /// round_up_multiple_count
#include "Memory/BlockAllocator.hpp"
#include "SceneGraph/MarkerTypeDepot.hpp"
/// Self header
#include "SceneGraph/TransformMarker.hpp"


TransformCreateParam::TransformCreateParam (
    const StaticStringIdT   _name_id,
    SceneObject &           _owner,
    TransformMarker * const _father)
:
    SuperT(_name_id, _owner),
    father(_father)
{

}



// MARK: == 变换属性分配器 ==
class TransformAllocator
{
public:
    static
    INLINE_FUNCTION
    TransformAllocator &
    ref ()
    {
        static TransformAllocator s_instance;
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
    TransformAllocator ()
    {
        static constexpr uint32_t MARKER_BYTE_SIZE = (uint32_t)sizeof(TransformMarker);
        /// 起始的变换属性个数
        static constexpr uint16_t TRANSFORM_COUNT  = 100;
        static constexpr uint32_t EXPECT_BYTE_SIZE = MARKER_BYTE_SIZE * TRANSFORM_COUNT;

        const bool is_initialized = m_allocator.initialize(
            MathUtility::round_up_multiple_count(EXPECT_BYTE_SIZE, BLOCK_ALLOCATOR_PAGE_SIZE), 2);
        UNUSED_VARIABLE(is_initialized);
        RUNTIME_ASSERT(is_initialized, "Can not initialize the allocator!!");
    }

    INLINE_FUNCTION
    ~TransformAllocator ()
    {
        m_allocator.release();
    }

private:
    typedef BlockAllocator<TransformMarker> AllocatorTypeT;

    AllocatorTypeT m_allocator;
};



// MARK: == 变换属性 ==
DEFINE_MARKER_TYPE_INFO(
    TransformMarker, TransformMarker::create, TransformMarker::destroy);


ObjectMarker *
TransformMarker::create (
    const CreateParameter & parameter)
{
    /// 申请内存
    void * const new_marker = TransformAllocator::ref().allocate();
    if (new_marker)
    {
        /// 构建实例
        const TransformCreateParam & create_param =
            (const TransformCreateParam&)parameter;
        new(new_marker)TransformMarker(create_param.owner, create_param.father);
    }

    return (ObjectMarker*)new_marker;
}


bool
TransformMarker::destroy (
    ObjectMarker * & marker)
{
    RUNTIME_ASSERT(marker, "Marker can not be NULL!!");

    TransformMarker * const transform_marker = (TransformMarker*)marker;
    /// 调用析构函数
    transform_marker->~TransformMarker();
    /// 释放内存
    const bool opcode = TransformAllocator::ref().destroy(transform_marker);
    /// 清除参考
    marker = nullptr;
    return opcode;
}


TransformMarker::TransformMarker (
    SceneObject &           owner,
    TransformMarker * const father)
:
    SuperT(ms_type_info.name_id(), owner)
{

}


TransformMarker::~TransformMarker ()
{

}
