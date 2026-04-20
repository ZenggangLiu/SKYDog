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
    Created:  19/04/26  @  9:53 PM
    FileName: NativeWriteStream.hpp @ RedSoUL Project
    History:
             - created by: 19/04/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "FileSystem/NativeFile.hpp"


/// WriteOnly文件流
///
class NativeWriteStream : public NativeFile
{
public:
    NativeWriteStream (
        const char * const absolute_file_name);

    ~NativeWriteStream ();

    NativeWriteStream &
    operator << (
        const char data);

    NativeWriteStream &
    operator << (
        const int8_t data);

    NativeWriteStream &
    operator << (
        const int16_t data);

    NativeWriteStream &
    operator << (
        const int32_t data);

    NativeWriteStream &
    operator << (
        const int64_t data);

    NativeWriteStream &
    operator << (
        const uint8_t data);

    NativeWriteStream &
    operator << (
        const uint16_t data);

    NativeWriteStream &
    operator << (
        const uint32_t data);

    NativeWriteStream &
    operator << (
        const uint64_t data);

    NativeWriteStream &
    operator << (
        const float data);

    NativeWriteStream &
    operator << (
        const double data);

    /// 直接输出指定字符串中的内容
    /// NOTE:
    /// - 无字符串长度信息
    NativeWriteStream &
    operator << (
        const char * const data);
};


/// 换行符
#if !defined(LINE_FEED)
#define LINE_FEED ("\n")
#endif