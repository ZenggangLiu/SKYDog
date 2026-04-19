/// System headers
#include <cstdio>  /// std::snprintf
#include <cstring> /// std::memcpy, std::strlen
/// Library headers
#include "Assert/RuntimeAssert.hpp"
/// Self header
#include "FileSystem/FileName.hpp"


bool
FileName::append_file_extension (
    const char * const file_name,
    const char * const extension,
    char * const       buffer_start,
    const uint32_t     buffer_size)
{
    RUNTIME_ASSERT( file_name,    "File name can not be NULL!!");
    RUNTIME_ASSERT(*file_name,    "File name can not be empty!!");
    RUNTIME_ASSERT( extension,    "Extension name can not be NULL!!");
    RUNTIME_ASSERT(*extension,    "Extension name can not be empty!!");
    RUNTIME_ASSERT( buffer_start, "Output buffer can not be NULL!!");
    RUNTIME_ASSERT( buffer_size,  "Output buffer size can not be 0!!");

    const uint32_t extension_length = (uint32_t)std::strlen(extension);

    /// 保存'.'的位置(如果指定文件名无扩展名, '.'的位置将设定在字符串尾)
    uint32_t dot_pos = (uint32_t)std::strlen(file_name);

    /// 从文件名尾部开始向前查找'.'分隔符
    for (int32_t c = (int32_t)(dot_pos - 1); c >= 0; --c)
    {
        /// 如果找到目录分隔符: 停止查找
        if (file_name[c] == '/' || file_name[c] == '\\')
        {
            break;
        }
        /// 如果找到'.'分隔符
        else if (file_name[c] == '.')
        {
            dot_pos = (uint32_t)c;
            break;
        }
    }
    RUNTIME_ASSERT(buffer_size >= dot_pos + extension_length + 1, /// '.'分隔符占一个字符
                   "Output buffer is too small!!");

    if (buffer_size >= dot_pos + extension_length + 1)
    {
        /// 复制'.'前方的字符串
        std::memcpy(buffer_start, file_name, dot_pos);
        /// 添加扩展名
        std::snprintf(buffer_start + dot_pos, buffer_size - dot_pos, ".%s", extension);
        return true;
    }
    else
    {
        return false;
    }
}
