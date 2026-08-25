#pragma once


// ********************
// *  == CHECKING ==  *
// ********************
//
/// 通过设置相应的宏为1，来Enable相应的测试
///
/// 检查Block Allocator
#define BLOCK_ALLOCATOR_CHECKING            1
/// 检查EXR图形文件
#define EXR_IMAGE_FILE_CHECKING             1
/// 检查Game Loop
#define GAME_LOOP_CHECKING                  1
/// 检查Static/Runtime Hashing
#define HASHING_CHECKING                    1
/// 检查Mathematic Utility
#define MATH_UTILITY_CHECKING               1
/// 检查原生目录控制
#define NATIVE_DIRECTORY_CHECKING           1
/// 检查原生读入/写出流
#define NATIVE_READ_WRITE_STREAM_CHECKING   1
/// 检查PLY几何体文件
#define PLY_MESH_FILE_CHECKING              1
/// 检查PPM图形文件
#define PPM_IMAGE_FILE_CHECKING             1
/// 检查随机数
#define RANDOM_NUMBER_CHECKING              1
/// 检查RMesh几何体文件
#define RENDER_MESH_FILE_CHECKING           1
/// 检查Render Texture
#define RENDER_TEXTURE_CHECKING             1
/// 检查RTexture贴图文件
#define RENDER_TEXTURE_FILE_CHECKING        1
/// 检查Ring Buffer
#define RING_BUFFER_CHECKING                1
/// 检查XXTEA编码/解码
#define XXTEA_CODEC_CHECKING                1


// ********************
// *  !! DISABLED !!  *
// ********************
///
#undef GAME_LOOP_CHECKING
