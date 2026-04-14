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


#include <stdint.h> /// uint32_t,...


struct AABB;
struct IndexedTriangle;


struct RenderMesh
{
    typedef const uint8_t *         ConstVertexPtrT;
    typedef const IndexedTriangle * ConstTrianglePtrT;

    /// 顶点列表: 要求16字节对齐
    ConstVertexPtrT const   vertex_list;
    /// 三角面列表: 无对齐要求
    ConstTrianglePtrT const triangle_list;
    /// 顶点个数
    const uint32_t          vertex_count;
    /// 顶点列表大小(字节数)
    const uint32_t          vertex_list_size;
    /// 三角面个数
    const uint32_t          triangle_count;
    /// 顶点Layout(VertexDataType类型的合集)
    const uint16_t          vertex_layout;
    /// 标记顶点/三角面列表是否为动态分配(TRUE)
    const uint16_t          is_dyn_allocated;

    RenderMesh (
        const AABB &            bound_box,
        const uint16_t          vertex_layout,
        const uint32_t          vertex_count,
        const uint32_t          triangle_count,
        const bool              is_dyn_allocated,
        ConstVertexPtrT const   vertex_list,
        ConstTrianglePtrT const triangle_list);
};
