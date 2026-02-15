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
    Created:  11/02/26  @  12:15 PM
    FileName: BuiltInTypes.hpp @ RedSoUL Project
    History:
             - created by: 11/02/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


#include <stdint.h> /// C++11 standard types


/// 布尔类型
typedef bool                            Bool;


// -- 字符类型 -- //
/// ASCII字符
typedef char                            ASCII;
/// UTF8字符
typedef unsigned char                   UTF8;
/// UTF16字符: 16位长
/// NOTE: 我们无法使用 UTF16 _utf16_code = u'🀀';
///       因为此字符u'🀀'需要两个UTF16编码
typedef char16_t                        UTF16;
/// UTF32字符：16位长Unicode
typedef char32_t                        UTF32;


// -- 有符号整数类型 -- //
/// 8位有符号整数
typedef int8_t                          SByte;
/// 16位有符号整数
typedef int16_t                         SShort;
/// 32位有符号整数
typedef int32_t                         SInt;
/// 64位有符号整数
typedef int64_t                         SLong;


// -- 无符号整数类型 -- //
/// 8位无符号整数
typedef uint8_t                         UByte;
/// 16位无符号整数
typedef uint16_t                        UShort;
/// 32位无符号整数
typedef uint32_t                        UInt;
/// 64位无符号整数
typedef uint64_t                        ULong;


// -- 浮点数类型 -- //
/// 16位浮点数
typedef struct _Real16 { UShort val; }  Half;
/// 32位浮点数
typedef float                           Float;
/// 64位浮点数
typedef double                          Double;


// --- 类型字节数目检查 --- //
static_assert(sizeof(SByte ) == 1, "Wrong DataType size!!");
static_assert(sizeof(UByte ) == 1, "Wrong DataType size!!");
static_assert(sizeof(SShort) == 2, "Wrong DataType size!!");
static_assert(sizeof(UShort) == 2, "Wrong DataType size!!");
static_assert(sizeof(SInt  ) == 4, "Wrong DataType size!!");
static_assert(sizeof(UInt  ) == 4, "Wrong DataType size!!");
static_assert(sizeof(SLong ) == 8, "Wrong DataType size!!");
static_assert(sizeof(ULong ) == 8, "Wrong DataType size!!");
static_assert(sizeof(Half  ) == 2, "Wrong DataType size!!");
static_assert(sizeof(Float ) == 4, "Wrong DataType size!!");
static_assert(sizeof(Double) == 8, "Wrong DataType size!!");


// --- Type Alias定义 --- //
/// 内存地址：64位长
#if !defined(MEMADDR)
    #define MEMADDR ULong
#endif /// !defined(MEMADDR)


/// NULL定义
#if !defined(NULL)
    #define NULL 0
#endif /// !defined(NULL)
