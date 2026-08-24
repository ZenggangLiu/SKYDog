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
    Created:  28/04/26  @  11:11 PM
    FileName: SoULFileHead.hpp @ RedSoUL Project
    History:
             - created by: 28/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint8_t
/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION, PACKED_STRUCT
#include "FileSystem/NativeReadStream.hpp"


/// Red SoUL文件头
///
/// +-----------------------+
/// |                       |
/// | == SoUL File Head ==  |
/// |                       |
/// +-----------------------+
/// | Magic Number          | <-- 四个字节(32位)
/// +-----------------------+
/// | Major Version         | <-- 一个字节(8位):主版本号
/// |-----------------------+
/// | Minor Version         | <-- 一个字节(8位):辅版本号
/// |-----------------------+
/// | File Flags            | <-- 二个字节(16位):文件标记
/// +-----------------------+
///
PACKED_STRUCT(SoULFileHead,
    const uint32_t magic_number;
    const uint8_t  major_version;
    const uint8_t  minor_version;
    const uint16_t file_flags;

    /// 从输入流创建实例
    INLINE_FUNCTION
    static
    bool
    create_from_stream (
        NativeReadStream & input_stream,
        SoULFileHead &     soul_file_head)
    {
        if (input_stream.is_opened())
        {
            return input_stream.read(
                (uint8_t*)&soul_file_head, sizeof(SoULFileHead),
                0, sizeof(SoULFileHead)) == sizeof(SoULFileHead);
        }
        else
        {
            return false;
        }
    }

    INLINE_FUNCTION
    SoULFileHead (
        const uint32_t _magic_number,
        const uint8_t  _major_version,
        const uint8_t  _minor_version,
        const uint16_t _file_flags)
    :
        magic_number(_magic_number),
        major_version(_major_version),
        minor_version(_minor_version),
        file_flags(_file_flags)
    {}
);
