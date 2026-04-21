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
    Created:  21/04/26  @  10:36 PM
    FileName: Layout_Pos_Colr_Norm_Uv.hpp @ RedSoUL Project
    History:
             - created by: 21/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint16_t
/// Library headers
#include "DataType/Float2D.hpp"
#include "DataType/Float3D.hpp"
#include "Render/VertexDataType.hpp"


/// 带有如下数据的顶点:
/// +------------+---------+----------+------+
/// | (POSITION) | (COLOR) | (NORMAL) | (UV) |
/// +------------+---------+----------+------+
///
struct Layout_Pos_Colr_Norm_Uv
{
    static constexpr uint16_t LAYOUT_DECL = VertexDataType::COLOR_DATA
                                          | VertexDataType::NORMAL_DATA
                                          | VertexDataType::POSITION_DATA
                                          | VertexDataType::TEXCOORD_DATA;

    float_3 position;
    float_3 color;
    float_3 normal;
    float_2 texcoord;
};
