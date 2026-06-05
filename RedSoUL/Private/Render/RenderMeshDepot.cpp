/// System headers
#include <cstdlib>  /// std::free
#include <cstdio>   /// std::snprintf
/// Library headers
#include "Collision/AABB.hpp"
#include "Assert/RuntimeAssert.hpp"
#include "IO/MeshFile/PlyFile.hpp"
#include "IO/MeshFile/RMeshFile.hpp"
#include "Memory/MemoryUtilities.hpp"
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


RenderMeshDepot &
RenderMeshDepot::ref ()
{
    static RenderMeshDepot s_instance;
    return s_instance;
}


const RenderMesh *
RenderMeshDepot::mesh_data (
    const RenderMeshIdT mesh_id) const
{
    RUNTIME_ASSERT(mesh_id != INVALID_RENDER_MESH_ID, "Mesh Id is invalid!!");

    ConstRenderMeshIteratorT cached_data = m_mesh_table.find(mesh_id);
    if (cached_data == m_mesh_table.end())
    {
        return nullptr;
    }
    else
    {
        return &cached_data->second;
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

    cache_mesh(
        MESH_ID, MESH_BOUND_BOX,
        VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
        false, (const uint8_t*)VERTEX_LIST, TRIANGLE_LIST);
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

    cache_mesh(
        MESH_ID, MESH_BOUND_BOX,
        VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
        false, (const uint8_t*)VERTEX_LIST, TRIANGLE_LIST);
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

    cache_mesh(
        MESH_ID, MESH_BOUND_BOX,
        VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
        false, (const uint8_t*)VERTEX_LIST, TRIANGLE_LIST);
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

    cache_mesh(
        MESH_ID, MESH_BOUND_BOX,
        VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
        false, (const uint8_t*)VERTEX_LIST, TRIANGLE_LIST);
    return MESH_ID;
}


RenderMeshIdT
RenderMeshDepot::create_unit_icosahedron ()
{
/// 如果需要创建顶点颜色数据, 将此宏定义为1
#define USE_VERTEX_COLOR 0

#if (USE_VERTEX_COLOR == 1)
    typedef struct Layout_Pos_Colr VertexLayoutT;
#else
    typedef struct Layout_Pos VertexLayoutT;
#endif

    /// 计算 Mesh Id
    static constexpr RenderMeshIdT MESH_ID =
        STATIC_RENDER_MESH_HASH(GENERATE_BUILTIN_MESH_NAME("UNIT_ICOSPHERE"));

    /// 参考:
    ///  http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html#comment-2736314361
    ///
    /// Golden Ratio(为了将顶点放置在球体上)
    /// 顶点使用{0, +/-S, +/-T}的排列组合来生成
    /// 这里:
    ///             5 - sqrt(5)
    /// - S = sqrt(─────────────) * 球半径
    ///                 10
    ///             5 + sqrt(5)
    /// - T = sqrt(─────────────) * 球半径
    ///                 10
    static constexpr float S_VALUE = 0.2628655561f;
    static constexpr float T_VALUE = 0.4253254042f;

    /// 包围盒
    static constexpr AABB MESH_BOUND_BOX
    {
        { -(T_VALUE + 0.01f), -(T_VALUE + 0.01f), -(T_VALUE + 0.01f) },
        { +(T_VALUE + 0.01f), +(T_VALUE + 0.01f), +(T_VALUE + 0.01f) }
    };

    /// 顶点列表
    static constexpr VertexLayoutT VERTEX_LIST[] =
    {
        /// YZ平面(蓝色平面): {0, +/-T, -/+S}: Y值比Z值大
        /// P0: <0, +T, -S>: 红
        {
            { 0.0f, +T_VALUE, -S_VALUE },
#if (USE_VERTEX_COLOR == 1)
            { 1.0f, +0.0f,    +0.0f    },
#endif
        },
        /// P1: <0, +T, +S>: 绿
        {
            { 0.0f, +T_VALUE, +S_VALUE },
#if (USE_VERTEX_COLOR == 1)
            { 0.0f, +1.0f,    +0.0f    },
#endif
        },
        /// P2: <0, -T, -S>: 蓝
        {
            { 0.0f, -T_VALUE, -S_VALUE },
#if (USE_VERTEX_COLOR == 1)
            { 0.0f, +0.0f,    +1.0f    },
#endif
        },
        /// P3: <0, -T, +S>: 黄
        {
            { 0.0f, -T_VALUE, +S_VALUE },
#if (USE_VERTEX_COLOR == 1)
            { 1.0f, +1.0f,    +0.0f    },
#endif
        },

        //// XY平面(绿色平面): {+/-T, -/+S, 0}: X值大于Y值
        /// P4: <+T, -S, 0>: 青
        {
            { +T_VALUE, -S_VALUE, 0.0f },
#if (USE_VERTEX_COLOR == 1)
            { +0.0f,    +1.0f,    1.0f },
#endif
        },
        /// P5: <+T, +S, 0>: 品红
        {
            { +T_VALUE, +S_VALUE, 0.0f },
#if (USE_VERTEX_COLOR == 1)
            { +1.0f,    +0.0f,    1.0f },
#endif
        },
        /// P6: <-T, -S, 0>: 橙
        {
            { -T_VALUE, -S_VALUE, 0.0f },
#if (USE_VERTEX_COLOR == 1)
            { +1.0f,    +0.5f,    0.0f },
#endif
        },
        /// P7: <-T, +S, 0>: 紫
        {
            { -T_VALUE, +S_VALUE, 0.0f },
#if (USE_VERTEX_COLOR == 1)
            { +0.5f,    +0.0f,    1.0f },
#endif
        },

        //// XZ平面(红色平面): {-/+S, 0, +/-T}: Z值大于Y值
        /// P8:  <-S, 0, +T>: 黄绿
        {
            { -S_VALUE, 0.0f, +T_VALUE },
#if (USE_VERTEX_COLOR == 1)
            { +0.5f,    1.0f, +0.0f    },
#endif
        },
        /// P9:  <+S, 0, +T>: 天蓝
        {
            { +S_VALUE, 0.0f, +T_VALUE },
#if (USE_VERTEX_COLOR == 1)
            { +0.0f,    0.5f, +1.0f    },
#endif
        },
        /// P10: <-S, 0, -T>: 粉红
        {
            { -S_VALUE, 0.0f, -T_VALUE },
#if (USE_VERTEX_COLOR == 1)
            { +1.0f,    0.5f, +0.75f   },
#endif
        },
        /// P11: <+S, 0, -T>: 灰
        {
            { +S_VALUE, 0.0f, -T_VALUE },
#if (USE_VERTEX_COLOR == 1)
            { +0.6f,    0.6f, +0.6f    }
#endif
        }
    };

    /// 三角面列表
    static const IndexedTriangle TRIANGLE_LIST[] =
    {
        /// - <0, 1, 2> --> <0, 2, 1>
        /// 
        /// P0处的5个三角面
        { 0, 5,  11 },
        { 0, 1,  5  },
        { 0, 7,  1  },
        { 0, 10, 7  },
        { 0, 11, 10 },

        /// 5个相邻的三角面
        { 1,  9, 5  },
        { 5,  4, 11 },
        { 11, 2, 10 },
        { 10, 6, 7  },
        { 7,  8, 1  },

        /// P3处的5个三角面
        { 3, 4, 9 },
        { 3, 2, 4 },
        { 3, 6, 2 },
        { 3, 8, 6 },
        { 3, 9, 8 },

        /// 5个相邻的三角面
        { 4, 5,  9 },
        { 2, 11, 4 },
        { 6, 10, 2 },
        { 8, 7,  6 },
        { 9, 1,  8 }
    };

    /// #vertex
    static constexpr uint16_t VERTEX_COUNT   = ARRAY_SIZE(VERTEX_LIST);
    /// #triangle
    static constexpr uint16_t TRIANGLE_COUNT = ARRAY_SIZE(TRIANGLE_LIST);

    cache_mesh(
        MESH_ID, MESH_BOUND_BOX,
        VertexLayoutT::LAYOUT_DECL, VERTEX_COUNT, TRIANGLE_COUNT,
        false, (const uint8_t*)VERTEX_LIST, TRIANGLE_LIST);
    return MESH_ID;
}


RenderMeshIdT
RenderMeshDepot::create_from_mesh_file (
    const char * const  abs_file_name,
    const RenderMeshIdT exp_mesh_id)
{
    /// 无此Mesh
    if (m_mesh_table.find(exp_mesh_id) == m_mesh_table.end())
    {
        AABB bound_box;
        uint16_t vertex_layout;
        uint32_t vertex_count;
        uint32_t triangle_count;
        uint8_t * vertex_list;
        IndexedTriangle * triangle_list;
        const bool opcode = RMeshFile::read_from(
            abs_file_name, exp_mesh_id, bound_box,
            vertex_layout, vertex_count, triangle_count,
            vertex_list, triangle_list);
        if (opcode == false)
        {
            return INVALID_RENDER_MESH_ID;
        }

        cache_mesh(exp_mesh_id, bound_box, vertex_layout,
                   vertex_count, triangle_count,
                   true, vertex_list, triangle_list);
    }

    return exp_mesh_id;
}


void
RenderMeshDepot::write_to_ply_file (
    const char * const  abs_file_name,
    const RenderMeshIdT mesh_id) const
{
    const ConstRenderMeshIteratorT render_mesh = m_mesh_table.find(mesh_id);
    if (render_mesh != m_mesh_table.end())
    {
        PlyFile::write_to(abs_file_name, render_mesh->second);
    }
}


void
RenderMeshDepot::write_to_mesh_file (
    const char * const  abs_file_name,
    const RenderMeshIdT mesh_id) const
{
    const ConstRenderMeshIteratorT render_mesh = m_mesh_table.find(mesh_id);
    if (render_mesh != m_mesh_table.end())
    {
        RMeshFile::write_to(abs_file_name, render_mesh->second);
    }
}


void
RenderMeshDepot::clear ()
{
    for (RenderMeshIteratorT mesh = m_mesh_table.begin();
         mesh != m_mesh_table.end(); ++mesh)
    {
        if (mesh->second.is_dyn_allocated)
        {
            MemoryUtility::aligned_free((void*)mesh->second.vertex_list);
            std::free((void*)mesh->second.triangle_list);
        }
    }
    m_mesh_table.clear();
}


RenderMeshDepot::RenderMeshDepot ()
{

}


RenderMeshDepot::~RenderMeshDepot ()
{
    clear();
}


void
RenderMeshDepot::cache_mesh (
    const RenderMeshIdT           mesh_id,
    const AABB &                  bound_box,
    const uint16_t                vertex_layout,
    const uint32_t                vertex_count,
    const uint32_t                triangle_count,
    const bool                    is_dyn_allocated,
    const uint8_t * const         vertex_list,
    const IndexedTriangle * const triangle_list)
{
    /// 无此Mesh
    if (m_mesh_table.find(mesh_id) == m_mesh_table.end())
    {
        m_mesh_table.insert(
        {
            mesh_id,
            RenderMesh(mesh_id, bound_box, vertex_layout,
                       vertex_count, triangle_count,
                       is_dyn_allocated, vertex_list, triangle_list)
        });
    }
}
