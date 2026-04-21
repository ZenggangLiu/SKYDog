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
#include "IO/MeshFile/PlyFile.hpp"
#include "Math/MathUtilities.hpp"   /// multiple_of
#include "Render/IndexedTriangle.hpp"
#include "Render/RenderMesh.hpp"
#include "Render/VertexLayout/Layout_Pos.hpp"
#include "Render/VertexLayout/Layout_Pos_Colr.hpp"
#include "Render/VertexLayout/Layout_Pos_Colr_Norm.hpp"
#include "Render/VertexLayout/Layout_Pos_Colr_Norm_Uv.hpp"
#include "Render/VertexLayout/Layout_Pos_Colr_Uv.hpp"
#include "Render/VertexLayout/Layout_Pos_Uv.hpp"
/// Self header
#include "Render/RenderMeshDepot.hpp"


#define GENERATE_BUILTIN_MESH_NAME(unique_name) "__/::@" unique_name "@::\\__"


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
unaligned_free (
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


void
RenderMeshDepot::write_to_ply_file (
    const char * const  abs_file_name,
    const RenderMeshIdT mesh_id) const
{
    /// 没有Cube
    const ConstRenderMeshIteratorT render_mesh = m_mesh_table.find(mesh_id);
    if (render_mesh != m_mesh_table.end())
    {
        PlyFile::write_to(abs_file_name, render_mesh->second);
    }
}


RenderMeshIdT
RenderMeshDepot::create_unit_square ()
{
    typedef struct Layout_Pos VertexLayoutT;

    /// 计算 Mesh Id
    static constexpr RenderMeshIdT MESH_ID =
        STATIC_RENDER_MESH_HASH(GENERATE_BUILTIN_MESH_NAME("UNIT_SQUARE"));

    /// 包围盒
    static constexpr AABB MESH_BOUND_BOX
    {
        { -0.5f, -0.001f, -0.5f },
        { +0.5f, +0.001f, +0.5f }
    };

    ///             ^ Z
    ///             |
    ///             |
    ///     1-------+-------3 (+0.5, 0, +0.5)
    ///     | *     |       |
    ///     |   *   |  II   |
    ///     |     * |       |
    ///     |       o-------+-------> X
    ///     |         *     |
    ///     |    I      *   |
    ///     |             * |
    ///     0---------------2
    /// (-0.5, 0, -0.5)
    ///
    /// 三角面I：  <0, 1, 2>
    /// 三角面II： <2 ,1, 3>

    /// 顶点列表
    static constexpr VertexLayoutT VERTEX_LIST[] =
    {
        /// 0
        {
            { -0.5f, +0.0f, -0.5f },
        },
        /// 1
        {
            { -0.5f, +0.0f, +0.5f },
        },
        /// 2
        {
            { +0.5f, +0.0f, -0.5f },
        },
        /// 3
        {
            { +0.5f, +0.0f, +0.5f },
        },
    };

    /// 三角面列表
    static const IndexedTriangle TRIANGLE_LIST[] =
    {
        { 0, 1, 2 },
        { 2, 1, 3 },
    };

    /// #vertex
    static constexpr uint16_t VERTEX_COUNT = ARRAY_SIZE(VERTEX_LIST);
    /// #triangle
    static constexpr uint16_t TRIANGLE_COUNT = ARRAY_SIZE(TRIANGLE_LIST);

    /// 没有Cube
    if (m_mesh_table.find(MESH_ID) == m_mesh_table.end())
    {
        cache_mesh(
            MESH_ID, MESH_BOUND_BOX,
            VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
            false, (ConstVertexPtrT)VERTEX_LIST, TRIANGLE_LIST);
    }

    return MESH_ID;
}


RenderMeshIdT
RenderMeshDepot::create_unit_square_uv ()
{
    typedef struct Layout_Pos_Uv VertexLayoutT;

    /// 计算 Mesh Id
    static constexpr RenderMeshIdT MESH_ID =
        STATIC_RENDER_MESH_HASH(GENERATE_BUILTIN_MESH_NAME("UNIT_SQUARE"));

    /// 包围盒
    static constexpr AABB MESH_BOUND_BOX
    {
        { -0.5f, -0.001f, -0.5f },
        { +0.5f, +0.001f, +0.5f }
    };

    ///             ^ Z
    ///             |
    ///             |
    ///     1-------+-------3 (+0.5, 0, +0.5)
    ///     | *     |       |
    ///     |   *   |  II   |
    ///     |     * |       |
    ///     |       o-------+-------> X
    ///     |         *     |
    ///     |    I      *   |
    ///     |             * |
    ///     0---------------2
    /// (-0.5, 0, -0.5)
    ///
    /// 三角面I：  <0, 1, 2>
    /// 三角面II： <2 ,1, 3>

    /// 顶点列表
    static constexpr VertexLayoutT VERTEX_LIST[] =
    {
        /// 0
        {
            { -0.5f, +0.0f, -0.5f },
            { +0.0f, +0.0f        }
        },
        /// 1
        {
            { -0.5f, +0.0f, +0.5f },
            { +0.0f, +1.0f        }
        },
        /// 2
        {
            { +0.5f, +0.0f, -0.5f },
            { +1.0f, +0.0f        }
        },
        /// 3
        {
            { +0.5f, +0.0f, +0.5f },
            { +1.0f, +1.0f        }
        }
    };

    /// 三角面列表
    static const IndexedTriangle TRIANGLE_LIST[] =
    {
        { 0, 1, 2 },
        { 2, 1, 3 }
    };

    /// #vertex
    static constexpr uint16_t VERTEX_COUNT = ARRAY_SIZE(VERTEX_LIST);
    /// #triangle
    static constexpr uint16_t TRIANGLE_COUNT = ARRAY_SIZE(TRIANGLE_LIST);

    /// 没有Cube
    if (m_mesh_table.find(MESH_ID) == m_mesh_table.end())
    {
        cache_mesh(
            MESH_ID, MESH_BOUND_BOX,
            VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
            false, (ConstVertexPtrT)VERTEX_LIST, TRIANGLE_LIST);
    }

    return MESH_ID;
}


RenderMeshIdT
RenderMeshDepot::create_unit_cube ()
{
    typedef struct Layout_Pos VertexLayoutT;

    /// 计算 Mesh Id
    static constexpr RenderMeshIdT MESH_ID =
        STATIC_RENDER_MESH_HASH(GENERATE_BUILTIN_MESH_NAME("UNIT_CUBE"));

    /// 包围盒
    static constexpr AABB MESH_BOUND_BOX
    {
        { -0.5f, -0.5f, -0.5f },
        { +0.5f, +0.5f, +0.5f }
    };

    /// 按照如下设定创建方块Cube:
    ///
    ///              ^ Y
    ///              |     / Z
    ///       6+-----|-------+7
    ///       /|     .   /  /|
    ///      / |        .  / |
    ///    2+-------------+3 |
    ///     |  |     |/   |  |
    ///     |  |     O----|.------> X
    ///     |  |          |  |
    ///     | 4+----------|--+5
    ///     | /           | /
    ///    0+-------------+1
    ///
    /// 顶点列表
    static constexpr VertexLayoutT VERTEX_LIST[] =
    {
        /// 0
        {
            { -0.50f, -0.50f, -0.50f }
        },
        /// 1
        {
            { +0.50f, -0.50f, -0.50f }
        },
        /// 2
        {
            { -0.50f, +0.50f, -0.50f }
        },
        /// 3
        {
            { +0.50f, +0.50f, -0.50f }
        },
        /// 4
        {
            { -0.50f, -0.50f, +0.50f }
        },
        /// 5
        {
            { +0.50f, -0.50f, +0.50f }
        },
        /// 6
        {
            { -0.50f, +0.50f, +0.50f }
        },
        /// 7
        {
            { +0.50f, +0.50f, +0.50f }
        }
    };

    /// 三角面列表
    static const IndexedTriangle TRIANGLE_LIST[] =
    {
        /// -X平面:
        /// 6+-------------+2
        ///  |           * |
        ///  |         *   |
        ///  |       *     |
        ///  |     *       |
        ///  |   *         |
        ///  | *           |
        /// 4+-------------+0
        /// <0, 4, 2>
        { 0, 4, 2 },
        /// <2, 4, 6>
        { 2, 4, 6 },

        /// +Y平面:
        /// 6+-------------+7
        ///  |           * |
        ///  |         *   |
        ///  |       *     |
        ///  |     *       |
        ///  |   *         |
        ///  | *           |
        /// 2+-------------+3
        /// <3, 2, 7>
        { 3, 2, 7 },
        /// <7, 2, 6>
        { 7, 2, 6 },

        /// -Z平面:
        /// 2+-------------+3
        ///  | *           |
        ///  |   *         |
        ///  |     *       |
        ///  |       *     |
        ///  |         *   |
        ///  |           * |
        /// 0+-------------+1
        /// <0, 2, 1>
        { 0, 2, 1 },
        /// <1, 2, 3>
        { 1, 2, 3 },

        /// -Y平面:
        /// 0+-------------+1
        ///  |           * |
        ///  |         *   |
        ///  |       *     |
        ///  |     *       |
        ///  |   *         |
        ///  | *           |
        /// 4+-------------+5
        /// <5, 4, 1>
        { 5, 4, 1 },
        /// <1, 4, 0>
        { 1, 4, 0 },

        /// +X平面:
        /// 3+-------------+7
        ///  |           * |
        ///  |         *   |
        ///  |       *     |
        ///  |     *       |
        ///  |   *         |
        ///  | *           |
        /// 1+-------------+5
        /// <5, 1, 7>
        { 5, 1, 7 },
        /// <7, 1, 3>
        { 7, 1, 3  },

        /// +Z平面:
        /// 7+-------------+6
        ///  | *           |
        ///  |   *         |
        ///  |     *       |
        ///  |       *     |
        ///  |         *   |
        ///  |           * |
        /// 5+-------------+4
        /// <5, 7, 4>
        { 5, 7, 4 },
        /// <4, 7, 6>
        { 4, 7, 6 }
    };

    /// #vertex
    static constexpr uint16_t VERTEX_COUNT   = ARRAY_SIZE(VERTEX_LIST);
    /// #triangle
    static constexpr uint16_t TRIANGLE_COUNT = ARRAY_SIZE(TRIANGLE_LIST);

    /// 没有Cube
    if (m_mesh_table.find(MESH_ID) == m_mesh_table.end())
    {
        cache_mesh(
            MESH_ID, MESH_BOUND_BOX,
            VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
            false, (ConstVertexPtrT)VERTEX_LIST, TRIANGLE_LIST);
    }

    return MESH_ID;
}


RenderMeshIdT
RenderMeshDepot::create_unit_cube_uv ()
{
    typedef struct Layout_Pos_Uv VertexLayoutT;

    /// 计算 Mesh Id
    static constexpr RenderMeshIdT MESH_ID =
        STATIC_RENDER_MESH_HASH(GENERATE_BUILTIN_MESH_NAME("UNIT_CUBE"));

    /// 包围盒
    static constexpr AABB MESH_BOUND_BOX
    {
        { -0.5f, -0.5f, -0.5f },
        { +0.5f, +0.5f, +0.5f }
    };

    /// 按照如下设定创建方块Cube:
    ///
    ///              ^ Y                          6Y       7Y
    ///              |     / Z                    +--------+
    ///       6+-----|-------+7                   |        |
    ///       /|     .   /  /|                    |   +Y   |
    ///      / |        .  / |           6X       2        3        7X       6Z
    ///    2+-------------+3 |           +--------+--------+--------+--------+
    ///     |  |     |/   |  |           |        |        |        |        |
    ///     |  |     O----|.------> X    |   -X   |   -Z   |   +X   |   +Z   |
    ///     |  |          |  |           |        |        |        |        |
    ///     | 4+----------|--+5          +--------+--------+--------+--------+
    ///     | /           | /            4x       0        1        5X       4Z
    ///    0+-------------+1                      |   -Y   |
    ///                                           |        |
    ///                                           +--------+
    ///                                           4Y       5Y
    ///
    /// 纹理坐标系:
    /// ^ V
    /// |
    /// |           6Y       7Y
    /// |           +--------+                    <--------+ 0.75
    /// |           |        |                             |
    /// |           |   +Y   |                             |
    /// |  6X       2        3        7X       6Z          |
    /// |  +--------+--------+--------+--------+  <--------+ 0.5
    /// |  |        |        |        |        |           |
    /// |  |   -X   |   +Z   |   +X   |   -Z   |           |
    /// |  |        |        |        |        |           |
    /// |  +--------+--------+--------+--------+  <--------+ 0.25
    /// |  4X       0        1        5X       4Z          |
    /// |           |   -Y   |                             |
    /// |           |        |                             |
    /// |           +--------+                    <--------+ 0
    /// +----------------------------------------------> U
    ///             4Y       5Y
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
        /// 0: 0
        {
            { -0.50f, -0.50f, -0.50f },
            { +0.25f, +0.25f         }
        },

        /// 1: 1
        {
            { +0.50f, -0.50f, -0.50f },
            { +0.50f, +0.25f         }
        },

        /// 2: 2
        {
            { -0.50f, +0.50f, -0.50f },
            { +0.25f, +0.50f         }
        },

        /// 3: 3
        {
            { +0.50f, +0.50f, -0.50f },
            { +0.50f, +0.50f         }
        },

        /// 4X: 4
        {
            { -0.50f, -0.50f, +0.50f },
            { +0.00f, +0.25f         }
        },
        /// 4Y: 5
        {
            { -0.50f, -0.50f, +0.50f },
            { +0.25f, +0.00f         }
        },
        /// 4Z: 6
        {
            { -0.50f, -0.50f, +0.50f },
            { +1.00f, +0.25f         }
        },

        /// 5X: 7
        {
            { +0.50f, -0.50f, +0.50f },
            { +0.75f, +0.25f         }
        },
        /// 5Y: 8
        {
            { +0.50f, -0.50f, +0.50f },
            { +0.50f, +0.00f         }
        },

        /// 6X: 9
        {
            { -0.50f, +0.50f, +0.50f },
            { +0.00f, +0.50f         }
        },
        /// 6Y: 10
        {
            { -0.50f, +0.50f, +0.50f },
            { +0.25f, +0.75f         }
        },
        /// 6Z: 11
        {
            { -0.50f, +0.50f, +0.50f },
            { +1.00f, +0.50f         }
        },

        /// 7X: 12
        {
            { +0.50f, +0.50f, +0.50f },
            { +0.75f, +0.50f         }
        },
        /// 7Y: 13
        {
            { +0.50f, +0.50f, +0.50f },
            { +0.50f, +0.75f         }
        }
    };

    /// 三角面列表
    static const IndexedTriangle TRIANGLE_LIST[] =
    {
        /// -X平面:
        /// 6X+-------------+2
        ///   |           * |
        ///   |         *   |
        ///   |       *     |
        ///   |     *       |
        ///   |   *         |
        ///   | *           |
        /// 4X+-------------+0
        /// <0, 4X, 2>
        { 0, 4, 2 },
        /// <2, 4X, 6X>
        { 2, 4, 9 },

        /// +Y平面:
        /// 6Y+-------------+7Y
        ///   |           * |
        ///   |         *   |
        ///   |       *     |
        ///   |     *       |
        ///   |   *         |
        ///   | *           |
        ///  2+-------------+3
        /// <3, 2, 7Y>
        { 3,  2, 13 },
        /// <7Y, 2, 6Y>
        { 13, 2, 10 },

        /// -Z平面:
        ///  2+-------------+3
        ///   | *           |
        ///   |   *         |
        ///   |     *       |
        ///   |       *     |
        ///   |         *   |
        ///   |           * |
        ///  0+-------------+1
        /// <0, 2, 1>
        { 0, 2, 1 },
        /// <1, 2, 3>
        { 1, 2, 3 },

        /// -Y平面:
        ///  0+-------------+1
        ///   |           * |
        ///   |         *   |
        ///   |       *     |
        ///   |     *       |
        ///   |   *         |
        ///   | *           |
        /// 4Y+-------------+5Y
        /// <5Y, 4Y, 1>
        { 8, 5, 1 },
        /// <1, 4Y, 0>
        { 1, 5, 0 },

        /// +X平面:
        ///  3+-------------+7X
        ///   |           * |
        ///   |         *   |
        ///   |       *     |
        ///   |     *       |
        ///   |   *         |
        ///   | *           |
        ///  1+-------------+5X
        /// <5X, 1, 7X>
        { 7,  1, 12 },
        /// <7X, 1, 3>
        { 12, 1, 3  },

        /// +Z平面:
        /// 7X+-------------+6Z
        ///   | *           |
        ///   |   *         |
        ///   |     *       |
        ///   |       *     |
        ///   |         *   |
        ///   |           * |
        /// 5X+-------------+4Z
        /// <5X, 7X, 4Z>
        { 7, 12, 6 },
        /// <4Z, 7X, 6Z>
        { 6, 12, 11 }
    };

    /// #vertex
    static constexpr uint16_t VERTEX_COUNT   = ARRAY_SIZE(VERTEX_LIST);
    /// #triangle
    static constexpr uint16_t TRIANGLE_COUNT = ARRAY_SIZE(TRIANGLE_LIST);

    /// 没有Cube
    if (m_mesh_table.find(MESH_ID) == m_mesh_table.end())
    {
        cache_mesh(
            MESH_ID, MESH_BOUND_BOX,
            VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
            false, (ConstVertexPtrT)VERTEX_LIST, TRIANGLE_LIST);
    }

    return MESH_ID;
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
