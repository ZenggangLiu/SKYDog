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
    Created:  5/03/26  @  11:52 AM
    FileName: StaticString.hpp @ RedSoUL Project
    History:
             - created by: 5/03/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Common/CompilerDefines.hpp" /// BUILD_MODE
#include "Text/StaticStringId.hpp"    /// StaticStringIdT


class StaticString
{
public:
    /// 获得空字符串的Id
    static
    StaticStringIdT
    get_empty_string_id ();

    /// 创建一个空字符串
    StaticString ();

    /// 使用Id创建一个字符串
    StaticString (
        const StaticStringIdT string_id);

     /// 使用给定的ASCII字符串来构建一个字符串
    StaticString (
        const char * const ascii_text);

    /// 使用给定的UTF8编码的字符串构建一个字符串
    StaticString (
        const uint8_t * const utf8_text);

    /// 获得字符串Id
    StaticStringIdT
    id () const;

    /// 获得字符串长度
    /// NOTE: 对于UTF8编码的字符串，返回的长度为此字符串对应的字节长度(不包括结尾处的\x0')
    uint16_t
    length () const;

    /// 获得此字符串的内容
    const uint8_t *
    data () const;

    /// 字符串比较操作符
    bool operator== (
        const StaticString other) const;

    bool operator!= (
        const StaticString other) const;


private:
    const StaticStringIdT m_string_id;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    /// Cached字符串指针。对于ASCII字符串，它的类型为const char*
    const uint8_t * const m_cached_string;
#endif
};
