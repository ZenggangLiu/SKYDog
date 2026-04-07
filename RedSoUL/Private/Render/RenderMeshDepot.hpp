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
    Created:  3/04/26  @  4:48 PM
    FileName: RenderMeshDepot.hpp @ RedSoUL Project
    History:
             - created by: 3/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
#include <unordered_map>
/// Library headers
#include "Render/RenderMeshId.hpp"


struct IndexedTriangle;
struct RenderMesh;


/// 渲染几何体仓库
///
/// +-------+-------+-----+-------+
/// |  Id1  |  Id2  | ... |  Idn  |
/// +-------+-------+-----+-------+
/// | Mesh1 | Mesh2 | ... | MeshN |
/// +-------+-------+-----+-------+
///
class RenderMeshDepot
{
public:
    /// 获得对此Depot的参考
    static
    RenderMeshDepot &
    ref ();

    /// 创建一个单位Cube
    ///
    ///              ^ Y
    ///              |     / Z
    ///        +-----|-------+ (0.5, 0.5, 0.5)
    ///       /|     .   /  /|
    ///      / |        .  / |
    ///     +-------------+  |
    ///     |  |     |/   |  |
    ///     |  |     O----|.--------> X
    ///     |  |          |  |
    ///     |  +----------|- +
    ///     | /           | /
    ///     +-------------+
    /// (-0.5, -0.5, -0.5)
    ///
    RenderMeshIdT
    create_unit_cube ();


private:
    typedef const uint8_t *         ConstVertexPtrT;
    typedef const IndexedTriangle * ConstTrianglePtrT;

     RenderMeshDepot ();
    ~RenderMeshDepot ();

    /// Cache新的RenderMesh
    ///
    /// @param[in]  is_dyn_allocated
    ///     标记顶点列表, 三角面列表是否为动态分配(TRUE)
    /// @param[in]  vertex_list
    ///     顶点数据列表(16字节对齐)
    /// @param[in]  triangle_list
    ///     三角面数据列表
    void
    cache_mesh (
        const RenderMeshIdT     mesh_id,
        const AABB &            bound_box,
        const uint32_t          vertex_layout,
        const uint32_t          vertex_count,
        const uint32_t          triangle_count,
        const bool              is_dyn_allocated,
        ConstVertexPtrT const   vertex_list,
        ConstTrianglePtrT const triangle_list);

private:
    typedef std::unordered_map < RenderMeshIdT,
                                 RenderMesh >   RenderMeshTableT;
    typedef RenderMeshTableT::iterator          RenderMeshIteratorT;

    RenderMeshTableT m_mesh_table;
};
