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
    Created:  25/04/26  @  4:49 PM
    FileName: RMeshFile.hpp @ RedSoUL Project
    History:
             - created by: 25/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Render/RenderMeshId.hpp"


struct AABB;
struct IndexedTriangle;
struct RenderMesh;


/// Render Mesh序列化文件
///
/// 文件Layout:
///
/// +-----------------------+
/// | Render Mesh File Head |
/// +-----------------------+
/// | Vertex Data List      | <-- Layout_Pos_XYZ
/// +-----------------------+
/// | Triangle Data List    | <-- IndexedTriangle
/// +-----------------------+
///
struct RMeshFile
{
    /// 读入一个RenderMesh文件
    static
    bool
    read_from (
        const char * const  abs_file_name,
        const RenderMeshIdT exp_mesh_id,
        AABB &              bound_box,
        uint16_t &          vertex_layout,
        uint32_t &          vertex_count,
        uint32_t &          triangle_count,
        uint8_t * &         vertex_list,
        IndexedTriangle * & triangle_list);

    /// 创建一个RenderMesh文件
    ///
    /// @param[in]  abs_file_name
    ///     绝对文件路径
    ///     NOTE: .rmesh文件扩展符将添加到给定的路径名上
    /// @param[in]  mesh_data
    ///     Mesh数据
    /// @return
    ///     True:   RenderMesh文件输出成功
    ///     False:  RenderMesh文件输出失败
    static
    bool
    write_to (
        const char * const abs_file_name,
        const RenderMesh & mesh_data);
};
