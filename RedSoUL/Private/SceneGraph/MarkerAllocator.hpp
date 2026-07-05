#pragma once


/// System headers
#include <stdint.h> /// uint16_t
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION, UNUSED_VARIABLE
#include "Math/MathUtilities.hpp"   /// round_up_count
#include "Memory/BlockAllocator.hpp"


/// Marker实例分配器
/// 按照不同的MarkerType, 使用BlockAllocator分配实例
///
template <typename MarkerType, int MarkerCount> /// <Marker类型, 起始Marker个数>
class MarkerAllocator
{
public:
    static
    INLINE_FUNCTION
    MarkerAllocator &
    ref ()
    {
        static MarkerAllocator s_instance;
        return s_instance;
    }

    INLINE_FUNCTION
    void *
    allocate ()
    {
        return m_allocator.allocate();
    }

    INLINE_FUNCTION
    bool
    deallocate (
        void * const alloc_addr)
    {
        return m_allocator.deallocate(alloc_addr);
    }

private:
    INLINE_FUNCTION
    MarkerAllocator ()
    {
        static constexpr uint32_t MARKER_BYTE_SIZE = (uint32_t)sizeof(MarkerType);
        static constexpr uint32_t EXPECT_BYTE_SIZE = MARKER_BYTE_SIZE * MarkerCount;

        const bool is_initialized = m_allocator.initialize(
            MathUtility::round_up_count(EXPECT_BYTE_SIZE, BLOCK_ALLOCATOR_PAGE_SIZE), 2);
        UNUSED_VARIABLE(is_initialized);
        RUNTIME_ASSERT(is_initialized, "Can not initialize the allocator!!");
    }

    INLINE_FUNCTION
    ~MarkerAllocator ()
    {
        m_allocator.release();
    }

private:
    typedef BlockAllocator<MarkerType> AllocatorTypeT;

    AllocatorTypeT m_allocator;
};
