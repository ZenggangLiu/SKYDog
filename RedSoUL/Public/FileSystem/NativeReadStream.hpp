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
    Created:  19/04/26  @  9:52 PM
    FileName: NativeReadStream.hpp @ RedSoUL Project
    History:
             - created by: 19/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "FileSystem/NativeFile.hpp"


/// ReadOnly文件流
///
class NativeReadStream : public NativeFile
{
public:
    NativeReadStream (
        const char * const absolute_file_name);

    ~NativeReadStream ();

    NativeReadStream &
    operator >> (
        char & data);

    NativeReadStream &
    operator >> (
        int8_t & data);

    NativeReadStream &
    operator >> (
        int16_t & data);

    NativeReadStream &
    operator >> (
        int32_t & data);

    NativeReadStream &
    operator >> (
        int64_t & data);

    NativeReadStream &
    operator >> (
        uint8_t & data);

    NativeReadStream &
    operator >> (
        uint16_t & data);

    NativeReadStream &
    operator >> (
        uint32_t & data);

    NativeReadStream &
    operator >> (
        uint64_t & data);

    NativeReadStream &
    operator >> (
        float & data);

    NativeReadStream &
    operator >> (
        double & data);
};
