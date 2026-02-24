#pragma once


// ********************
// *      检测配置      *
// ********************
//
/// 通过设置相应的宏为1，来Enable相应的测试
///
/// 检查Ring Buffer
#define RING_BUFFER_CHECKING                1
/// 检查XXTEA编码
#define XXTEA_CHECKING                      1


// ********************
// *   禁止的测试定义    *
// ********************
///
#undef MEMORY_HELPER_CHECKING
