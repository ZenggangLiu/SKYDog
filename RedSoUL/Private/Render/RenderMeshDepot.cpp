#include "Common/PlatformDefines.hpp"
/// System headers
#include <cstdio>   /// std::snprintf
#if (OS_TYPE == OS_TYPE_WIN)
#include <cstdlib>  /// _aligned_malloc, _aligned_free
#else
#include <stdlib.h> /// posix_memalign
#endif
/// Library headers
#include "Collision/AABB.hpp"
#include "Common/CommonDefines.hpp" /// FOUR_CC
#include "Hashing/RuntimeHash.hpp"
#include "Hashing/StaticStringHash.hpp"
#include "Math/MathUtilities.hpp"   /// multiple_of
#include "Render/IndexedTriangle.hpp"
#include "Render/RenderMesh.hpp"
#include "Render/VertexLayout/Layout_Pos_Uv.hpp"
/// Self header
#include "Render/RenderMeshDepot.hpp"


#define GENERATE_BUILTIN_MESH_NAME(unique_name) "__/::@" unique_name "@::\__"


#define USE_ALIGNED_ALLOC_FUNC 0
#if (USE_ALIGNED_ALLOC_FUNC == 1)
static
uint8_t *
aligned_alloc (
    const uint32_t alloc_size,
    const uint32_t alignment)
{
#if (OS_TYPE == OS_TYPE_WIN)
    return (uint8_t*)_aligned_malloc(alloc_size, alignment);
#else /// macOS, iOS, Linux
    void * alloc_addr = nullptr;
    const int op_code = posix_memalign(&alloc_addr, alignment, alloc_size);
    if (op_code == 0)
    {
        return (uint8_t*)alloc_addr;
    }
    else
    {
        return nullptr;
    }
#endif
}
#endif /// (USE_ALIGNED_ALLOC_FUNC == 1)


static
void
aligned_free (
    const void * const alloc_addr)
{
#if (OS_TYPE == OS_TYPE_WIN)
    _aligned_free((void*)alloc_addr);
#else /// macOS, iOS, Linux
    free((void*)alloc_addr);
#endif
}


static
void
unaligned_free(
    const void * const alloc_addr)
{
    free((void*)alloc_addr);
}


RenderMeshDepot &
RenderMeshDepot::ref ()
{
    static RenderMeshDepot s_instance;
    return s_instance;
}


RenderMeshIdT
RenderMeshDepot::create_unit_cube ()
{
    typedef struct Layout_Pos_Uv VertexLayoutT;

    /// 计算 Mesh Id
    static constexpr RenderMeshIdT CUBE_MESH_ID = STATIC_RENDER_MESH_HASH( "UNIT_CUBE_MESH");

    /// 包围盒
    static constexpr AABB CUBE_BOUND_BOX
    {
        { -0.5, -0.5, -0.5 },
        { +0.5, +0.5, +0.5 }
    };

    /// 按照如下设定创建方块Cube:
    ///
    ///              ^ Y                          2Y       3Y
    ///              |     / Z                    +--------+
    ///       6+-----|-------+7                   |        |
    ///       /|     .   /  /|                    |   +Y   |
    ///      / |        .  / |           2X       6        7        3X       2Z
    ///    2+-------------+3 |           +--------+--------+--------+--------+
    ///     |  |     |/   |  |           |        |        |        |        |
    ///     |  |     O----|.------> X    |   -X   |   +Z   |   +X   |   -Z   |
    ///     |  |          |  |           |        |        |        |        |
    ///     | 4+----------|--+5          +--------+--------+--------+--------+
    ///     | /           | /            0X       4        5        1X       0Z
    ///    0+-------------+1                      |   -Y   |
    ///                                           |        |
    ///                                           +--------+
    ///                                           0Y       1Y
    ///
    /// 纹理坐标系:
    /// ^ V
    /// |
    /// |           2Y       3Y
    /// |           +--------+                    <--------+ 0.75
    /// |           |        |                             |
    /// |           |   +Y   |                             |
    /// |  2X       6        7        3X       2Z          |
    /// |  +--------+--------+--------+--------+  <--------+ 0.5
    /// |  |        |        |        |        |           |
    /// |  |   -X   |   +Z   |   +X   |   -Z   |           |
    /// |  |        |        |        |        |           |
    /// |  +--------+--------+--------+--------+  <--------+ 0.25
    /// |  0X       4        5        1X       0Z          |
    /// |           |   -Y   |                             |
    /// |           |        |                             |
    /// |           +--------+                    <--------+ 0
    /// +----------------------------------------------> U
    ///             0Y       1Y
    ///
    ///    ^        ^        ^        ^        ^
    ///    |        |        |        |        |
    ///    |        |        |        |        |
    ///    +--------+--------+--------+--------+
    ///    0        0.25     0.5      0.75     1
    ///
    /// 顶点列表
    static constexpr VertexLayoutT VERTEX_LIST[] =
    {
        /// 0X: 0
        {
            { -0.50f, -0.50f, -0.50f },
            { +0.00f, +0.25f         },
        },
        /// 0Y: 1
        {
            { -0.50f, -0.50f, -0.50f },
            { +0.25f, +0.00f         },
        },
        /// 0Z: 2
        {
            { -0.50f, -0.50f, -0.50f },
            { +1.00f, +0.25f         },
        },

        /// 1X: 3
        {
            { +0.50f, -0.50f, -0.50f },
            { +0.75f, +0.25f         },
        },
        /// 1Y: 4
        {
            { +0.50f, -0.50f, -0.50f },
            { +0.50f, +0.00f         },
        },

        /// 2X: 5
        {
            { -0.50f, +0.50f, -0.50f },
            { +0.00f, +0.50f         },
        },
        /// 2Y: 6
        {
            { -0.50f, +0.50f, -0.50f },
            { +0.25f, +0.75f         },
        },
        /// 2Z: 7
        {
            { -0.50f, +0.50f, -0.50f },
            { +1.00f, +0.50f         },
        },

        /// 3X: 8
        {
            { +0.50f, +0.50f, -0.50f },
            { +0.75f, +0.50f         },
        },
        /// 3Y: 9
        {
            { +0.50f, +0.50f, -0.50f },
            { +0.50f, +0.75f         },
        },

        /// 4: 10
        {
            { -0.50f, -0.50f, +0.50f },
            { +0.25f, +0.25f         },
        },

        /// 5: 11
        {
            { +0.50f, -0.50f, +0.50f },
            { +0.50f, +0.25f         },
        },

        /// 6: 12
        {
            { -0.50f, +0.50f, +0.50f },
            { +0.25f, +0.50f         },
        },

        /// 7: 13
        {
            { +0.50f, +0.50f, +0.50f },
            { +0.50f, +0.50f         },
        },
    };

    /// 三角面列表
    static const IndexedTriangle TRIANGLE_LIST[] =
    {
        /// -X平面:
        /// 2X+-------------+6
        ///   | *           |
        ///   |   *         |
        ///   |     *       |
        ///   |       *     |
        ///   |         *   |
        ///   |           * |
        /// 0X+-------------+4
        /// <0X, 2X, 4>
        { 0,  5, 10 },
        /// <4, 2X, 6>
        { 10, 5, 12 },

        /// +Y平面:
        /// 2Y+-------------+3Y
        ///   | *           |
        ///   |   *         |
        ///   |     *       |
        ///   |       *     |
        ///   |         *   |
        ///   |           * |
        ///  6+-------------+7
        /// <6, 2Y, 7>
        { 12, 6, 13 },
        /// <7, 2Y, 3Y>
        { 13, 6, 9  },

        /// +Z平面:
        ///  6+-------------+7
        ///   | *           |
        ///   |   *         |
        ///   |     *       |
        ///   |       *     |
        ///   |         *   |
        ///   |           * |
        ///  4+-------------+5
        /// <4, 6, 5>
        { 10, 12, 11 },
        /// <5, 6, 7>
        { 11, 12, 13 },

        /// -Y平面:
        ///  4+-------------+5
        ///   | *           |
        ///   |   *         |
        ///   |     *       |
        ///   |       *     |
        ///   |         *   |
        ///   |           * |
        /// 0Y+-------------+1Y
        /// <0Y, 4, 1Y>
        { 1, 10, 4  },
        /// <1Y, 4, 5>
        { 4, 10, 11 },

        /// +X平面:
        ///  7+-------------+3X
        ///   | *           |
        ///   |   *         |
        ///   |     *       |
        ///   |       *     |
        ///   |         *   |
        ///   |           * |
        ///  5+-------------+1X
        /// <5, 7, 1X>
        { 11, 13, 3 },
        /// <1X, 7, 3X>
        { 3,  13, 8 },

        /// -Z平面:
        /// 3X+-------------+2Z
        ///   | *           |
        ///   |   *         |
        ///   |     *       |
        ///   |       *     |
        ///   |         *   |
        ///   |           * |
        /// 1X+-------------+0Z
        /// <1X, 3X, 0Z>
        { 3, 8, 2 },
        /// <0Z, 3X, 2Z>
        { 2, 8, 7 },
    };

    /// #vertex
    static constexpr uint16_t VERTEX_COUNT   = ARRAY_SIZE(VERTEX_LIST);
    /// #triangle
    static constexpr uint16_t TRIANGLE_COUNT = ARRAY_SIZE(TRIANGLE_LIST);

    /// 没有Cube
    if (m_mesh_table.find(CUBE_MESH_ID) == m_mesh_table.end())
    {
        cache_mesh(
            CUBE_MESH_ID, CUBE_BOUND_BOX,
            VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
            false, (ConstVertexPtrT)VERTEX_LIST, TRIANGLE_LIST);
    }

    return CUBE_MESH_ID;
}


RenderMeshDepot::RenderMeshDepot ()
{

}


RenderMeshDepot::~RenderMeshDepot ()
{
    for(RenderMeshIteratorT mesh = m_mesh_table.begin();
        mesh != m_mesh_table.end(); ++mesh)
    {
        if (mesh->second.is_dyn_allocated)
        {
            aligned_free(mesh->second.vertex_list);
            unaligned_free(mesh->second.triangle_list);
        }
    }
    m_mesh_table.clear();
}


void
RenderMeshDepot::cache_mesh (
    const RenderMeshIdT     mesh_id,
    const AABB &            bound_box,
    const uint32_t          vertex_layout,
    const uint32_t          vertex_count,
    const uint32_t          triangle_count,
    const bool              is_dyn_allocated,
    ConstVertexPtrT const   vertex_list,
    ConstTrianglePtrT const triangle_list)
{
    m_mesh_table.insert(
    {
        mesh_id,
        RenderMesh(bound_box, vertex_layout, vertex_count, triangle_count,
                   is_dyn_allocated, vertex_list, triangle_list)
    });
}
