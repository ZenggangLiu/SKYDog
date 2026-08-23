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
    Created:  5/04/26  @  5:42 PM
    FileName: RenderMesh.hpp @ RedSoUL Project
    History:
             - created by: 5/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Collision/AABB.hpp"
#include "Render/RenderMeshId.hpp"


struct IndexedTriangle;


struct RenderMesh
{
    /// 顶点数据列表: 要求16字节对齐
    const uint8_t * const         vertex_list;
    /// 三角面列表:   无对齐要求
    const IndexedTriangle * const triangle_list;
    /// Mesh Id
    const RenderMeshIdT           mesh_id;
    /// 包围盒
    const AABB                    bound_box;
    /// 顶点数据列表大小(字节数)
    const uint32_t                vertex_list_size;
    /// 三角面列表大小(字节数)
    const uint32_t                triangle_list_size;
    /// 顶点个数
    const uint32_t                vertex_count;
    /// 三角面个数
    const uint32_t                triangle_count;
    /// 顶点Layout(VertexDataType类型的合集)
    const uint16_t                vertex_layout;
    /// 标记顶点/三角面列表是否为动态分配(True)
    const bool                    is_dyn_allocated;

    RenderMesh (
        const RenderMeshIdT           mesh_id,
        const AABB &                  bound_box,
        const uint16_t                vertex_layout,
        const uint32_t                vertex_count,
        const uint32_t                triangle_count,
        const bool                    is_dyn_allocated,
        const uint8_t * const         vertex_list,
        const uint32_t                vertex_list_size,
        const IndexedTriangle * const triangle_list,
        const uint32_t                triangle_list_size);
};
