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
    Created:  21/06/26  @  10:17 PM
    FileName: CameraAllocator.hpp @ RedSoUL Project
    History:
             - created by: 21/06/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint16_t
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION, UNUSED_VARIABLE
#include "Math/MathUtilities.hpp"   /// round_up_count
#include "Memory/BlockAllocator.hpp"


/// 按照不同的CameraType, 使用BlockAllocator分配实例
template <typename CameraType>
class CameraAllocator
{
public:
    static
    INLINE_FUNCTION
    CameraAllocator &
    ref ()
    {
        static CameraAllocator s_instance;
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
    CameraAllocator ()
    {
        static constexpr uint32_t MARKER_BYTE_SIZE = (uint32_t)sizeof(CameraType);
        static constexpr uint16_t CAMERA_COUNT     = 4;
        static constexpr uint32_t EXPECT_BYTE_SIZE = MARKER_BYTE_SIZE * CAMERA_COUNT;

        const bool is_initialized = m_allocator.initialize(
            MathUtility::round_up_count(EXPECT_BYTE_SIZE, BLOCK_ALLOCATOR_PAGE_SIZE), 2);
        UNUSED_VARIABLE(is_initialized);
        RUNTIME_ASSERT(is_initialized, "Can not initialize the allocator!!");
    }

    INLINE_FUNCTION
    ~CameraAllocator ()
    {
        m_allocator.release();
    }

private:
    typedef BlockAllocator<CameraType> AllocatorTypeT;

    AllocatorTypeT m_allocator;
};
