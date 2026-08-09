/// System headers
#include <cstdint> /// uintptr_t
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CommonDefines.hpp" /// ALIGN_UP
#include "Render/IndexedTriangle.hpp"
#include "Render/VertexDataType.hpp"
#include "Render/VertexLayout/Layout_Pos.hpp"
#include "Render/VertexLayout/Layout_Pos_Colr.hpp"
#include "Render/VertexLayout/Layout_Pos_Colr_Norm.hpp"
#include "Render/VertexLayout/Layout_Pos_Colr_Norm_Uv.hpp"
#include "Render/VertexLayout/Layout_Pos_Colr_Uv.hpp"
#include "Render/VertexLayout/Layout_Pos_Norm.hpp"
#include "Render/VertexLayout/Layout_Pos_Norm_Uv.hpp"
#include "Render/VertexLayout/Layout_Pos_Uv.hpp"
/// Self header
#include "Render/RenderMesh.hpp"


// MARK: == Helpers ==
/// 计算顶点数据列表大小(字节数)
static
uint32_t
calc_vertex_list_size (
    const uint16_t vertex_layout,
    const uint32_t vertex_count)
{
    RUNTIME_ASSERT(vertex_layout & VertexDataType::POSITION_DATA,
                   "Position data is ALWAYS required!!");

    switch (vertex_layout)
    {
        /// 只有Pos数据
        case Layout_Pos::LAYOUT_DECL:
        {
            return (uint32_t)(sizeof(Layout_Pos) * vertex_count);
        }

        /// Pos数据 + Color数据
        case Layout_Pos_Colr::LAYOUT_DECL:
        {
            return (uint32_t)(sizeof(Layout_Pos_Colr) * vertex_count);
        }

        /// Pos数据 + Normal数据
        case Layout_Pos_Norm::LAYOUT_DECL:
        {
            return (uint32_t)(sizeof(Layout_Pos_Norm) * vertex_count);
        }

        /// Pos数据 + UV数据
        case Layout_Pos_Uv::LAYOUT_DECL:
        {
            return (uint32_t)(sizeof(Layout_Pos_Uv) * vertex_count);
        }

        /// Pos数据 + Color数据 + Normal数据
        case Layout_Pos_Colr_Norm::LAYOUT_DECL:
        {
            return (uint32_t)(sizeof(Layout_Pos_Colr_Norm) * vertex_count);
        }

        /// Pos数据 + Color数据 + UV数据
        case Layout_Pos_Colr_Uv::LAYOUT_DECL:
        {
            return (uint32_t)(sizeof(Layout_Pos_Colr_Uv) * vertex_count);
        }

        /// Pos数据 + Normal数据 + UV数据
        case Layout_Pos_Norm_Uv::LAYOUT_DECL:
        {
            return (uint32_t)(sizeof(Layout_Pos_Norm_Uv) * vertex_count);
        }

        /// Pos数据 + Color数据 + Normal数据 + UV数据
        case Layout_Pos_Colr_Norm_Uv::LAYOUT_DECL:
        {
            return (uint32_t)(sizeof(Layout_Pos_Colr_Norm_Uv) * vertex_count);
        }

        default:
        {
            RUNTIME_ASSERT(false, "Unknown Vertex Layout type!!");
            return 0;
        }
    }
}


/// 计算三角面列表的大小(字节数)
static
uint32_t
calc_triangle_list_size (
    const uint32_t triangle_count)
{
    return sizeof(IndexedTriangle) * triangle_count;
}



// MARK: == RenderMesh ==
RenderMesh::RenderMesh (
    const RenderMeshIdT           _mesh_id,
    const AABB &                  _bound_box,
    const uint16_t                _vertex_layout,
    const uint32_t                _vertex_count,
    const uint32_t                _triangle_count,
    const bool                    _is_dyn_allocated,
    const uint8_t * const         _vertex_list,
    const IndexedTriangle * const _triangle_list)
:
    vertex_list(_vertex_list),
    triangle_list(_triangle_list),
    mesh_id(_mesh_id),
    bound_box(_bound_box),
    vertex_list_size(calc_vertex_list_size(_vertex_layout, _vertex_count)),
    triangle_list_size(calc_triangle_list_size(_triangle_count)),
    vertex_count(_vertex_count),
    triangle_count(_triangle_count),
    vertex_layout(_vertex_layout),
    is_dyn_allocated(_is_dyn_allocated)
{
    RUNTIME_ASSERT(_mesh_id != INVALID_RENDER_MESH_ID, "Mesh Id is invalid!!");
    RUNTIME_ASSERT(
        (uintptr_t)_vertex_list == ALIGN_UP((uintptr_t)_vertex_list, 16),
        "Wrong alignment(16 bytes aligment required)!!");
}
