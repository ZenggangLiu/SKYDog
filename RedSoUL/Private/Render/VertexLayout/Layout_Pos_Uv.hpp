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
    Created:  4/04/26  @  3:45 PM
    FileName: Layout_Pos_Uv.hpp @ RedSoUL Project
    History:
             - created by: 4/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint16_t
/// Library headers
#include "DataType/Float2D.hpp"
#include "DataType/Float3D.hpp"
#include "Render/VertexDataType.hpp"


/// 带有如下数据的顶点:
/// - 位置数据
/// - 纹理数据
struct Layout_Pos_Uv
{
    static constexpr uint16_t LAYOUT_DECL = VertexDataType::POSITION_DATA
                                          | VertexDataType::TEXCOORD_DATA;

    float_3 position;
    float_2 texcoord;
};
