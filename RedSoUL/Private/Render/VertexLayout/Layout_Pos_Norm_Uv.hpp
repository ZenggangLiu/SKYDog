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
    Created:  26/07/26  @  5:48 PM
    FileName: Layout_Pos_Norm_Uv.hpp @ RedSoUL Project
    History:
             - created by: 26/07/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint16_t
/// Library headers
#include "DataType/Float2D.hpp"
#include "DataType/Float3D.hpp"
#include "Render/VertexDataType.hpp"


/// 带有如下数据的顶点:
/// +------------+----------+------+
/// | (POSITION) | (NORMAL) | (UV) |
/// +------------+----------+------+
///
struct Layout_Pos_Norm_Uv
{
    static constexpr uint16_t LAYOUT_DECL = VertexDataType::NORMAL_DATA
                                          | VertexDataType::POSITION_DATA
                                          | VertexDataType::TEXCOORD_DATA;

    float_3 position;
    float_3 normal;
    float_2 texcoord;
};
