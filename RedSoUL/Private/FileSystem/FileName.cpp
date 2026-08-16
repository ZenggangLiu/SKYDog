/// System headers
#include <cstdio>  /// std::snprintf
#include <cstring> /// std::memcpy, std::strlen
/// Library headers
#include "Assert/RuntimeAssert.hpp"
/// Self header
#include "FileSystem/FileName.hpp"


int32_t
FileName::base_name (
    const char * const file_name,
    char * const       buffer_start,
    const uint32_t     buffer_size)
{
    RUNTIME_ASSERT( file_name,    "File name can not be NULL!!");
    RUNTIME_ASSERT(*file_name,    "File name can not be empty!!");
    RUNTIME_ASSERT( buffer_start, "Output buffer can not be NULL!!");
    RUNTIME_ASSERT( buffer_size,  "Output buffer size can not be 0!!");

    int32_t base_name_length = (int32_t)std::strlen(file_name);

    /// 从文件名尾部开始向前查找'.'分隔符
    for (int32_t c = base_name_length - 1; c >= 0; --c)
    {
        /// 如果找到目录分隔符: 停止查找
        if (file_name[c] == '/' || file_name[c] == '\\')
        {
            break;
        }
        /// 如果找到'.'分隔符
        else if (file_name[c] == '.')
        {
            base_name_length = c;
            break;
        }
    }
    RUNTIME_ASSERT(buffer_size >= (uint32_t)base_name_length + 1, /// 字符串结束符'\0'占用一个字节
                   "Output buffer is too small!!");

    if (buffer_size >= (uint32_t)base_name_length + 1) /// 字符串结束符'\0'占用一个字节
    {
        /// 复制'.'前方的字符串
        std::memcpy(buffer_start, file_name, base_name_length);
        /// 保存结束符'\0'
        buffer_start[base_name_length] = 0;
        return base_name_length;
    }
    else
    {
        return -1;
    }
}


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
    const int32_t  base_name_length = base_name(file_name, buffer_start, buffer_size);
    if (base_name_length > 0 &&  /// '.' 与 '\0'字符各占一个空间
        buffer_size >= (uint32_t)base_name_length + extension_length + 2) /// '.', '\0'
    {
        /// 添加扩展名
        std::snprintf(
            buffer_start + base_name_length,
            buffer_size  - base_name_length, ".%s", extension);
        return true;
    }
    else
    {
        return false;
    }
}
