#pragma once


// ********************
// *  == CHECKING ==  *
// ********************
//
/// 通过设置相应的宏为1，来Enable相应的测试
///
/// 检查Game Loop
#define GAME_LOOP_CHECKING                  1
// 检查Static/Runtime Hashing
#define HASHING_CHECKING                    1
/// 检查Mathematic Utility
#define MATH_UTILITY_CHECKING               1
/// 检查Ring Buffer
#define RING_BUFFER_CHECKING                1
/// 检查XXTEA编码/解码
#define XXTEA_CODEC_CHECKING                1


// ********************
// *  !! DISABLED !!  *
// ********************
///
#undef GAME_LOOP_CHECKING