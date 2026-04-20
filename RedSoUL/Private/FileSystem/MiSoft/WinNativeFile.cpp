#include "Common/PlatformDefines.hpp" /// OS_TYPE
#if (OS_TYPE == OS_TYPE_WIN)

/// System headers
#include <cstring> /// std::strlen
#include <Windows.h>
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "FileSystem/MiSoft/WCharHelper.hpp"
#include "FileSystem/MiSoft/WinFileHandle.hpp"
#include "FileSystem/NativeDirectory.hpp"
/// Self header
#include "FileSystem/NativeFile.hpp"


/// 获取真正的文件句柄
#define REAL_FILE_HANDLE (((WinFileHandle*)m_file_handle)->real_handle)


uint32_t
NativeFile::file_length (
    const char * const absolute_file_name)
{
    RUNTIME_ASSERT( absolute_file_name, "File name can not be NULL!!");
    RUNTIME_ASSERT(*absolute_file_name, "File name can not be empty!!");

    NativeFile file_io;
    file_io.open(absolute_file_name, AccessMode::READ_ONLY_ACCESS_MODE);
    if (file_io.is_opened())
    {
        const uint32_t io_length = file_io.file_length();
        file_io.close();

        return io_length;
    }
    else
    {
        return 0;
    }
}


bool
NativeFile::does_file_exist (
    const char * const absolute_file_name)
{
    RUNTIME_ASSERT( absolute_file_name, "File name can not be NULL!!");
    RUNTIME_ASSERT(*absolute_file_name, "File name can not be empty!!");

#if defined(UNICODE)
    std::wstring wchar_file_name;
    if (WCharHelper::utf8_to_wchar(absolute_file_name, wchar_file_name) == false)
    {
        return false;
    }

    /// 返回INVALID_FILE_ATTRIBUTES, 如果失败
    DWORD file_attr = GetFileAttributes(wchar_file_name.c_str());

#else
    /// 返回INVALID_FILE_ATTRIBUTES, 如果失败
    DWORD file_attr = GetFileAttributes(absolute_file_name);
#endif /// defined(UNICODE)

    if (file_attr == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }
    else
    {
        return (file_attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
    }
}


bool
NativeFile::delete_file (
    const char * const absolute_file_name)
{
    RUNTIME_ASSERT( absolute_file_name, "File name can not be NULL!!");
    RUNTIME_ASSERT(*absolute_file_name, "File name can not be empty!!");

#if defined(UNICODE)
    std::wstring wchar_file_name;
    if (WCharHelper::utf8_to_wchar(absolute_file_name, wchar_file_name) == false)
    {
        return false;
    }
    else
    {
        /// 返回非零, 如果成功
        return DeleteFile(wchar_file_name.c_str());
    }

#else
    return DeleteFile(absolute_file_name);
#endif /// defined(UNICODE)
}


NativeFile::NativeFile ()
:
    m_file_handle(nullptr),
    m_file_length(0),
    m_cursor_pos(0),
    m_is_file_opened(false)
{

}


NativeFile::~NativeFile ()
{
    close();
}


bool
NativeFile::open (
    const char * const absolute_file_name,
    const AccessMode   access_mode)
{
    RUNTIME_ASSERT( absolute_file_name, "File name can not be NULL!!");
    RUNTIME_ASSERT(*absolute_file_name, "File name can not be empty!!");

    /// 对于只写/读写方式, 我们必须确保其父目录存在
    if (access_mode != AccessMode::READ_ONLY_ACCESS_MODE)
    {
        char folder_name[MAX_PATH + 1] = { 0 };

        const uint32_t file_name_length = (uint32_t)std::strlen(absolute_file_name);
        uint32_t last_slash_pos = 0;
        /// 从后向前查找分割符：'/' OR '\'
        for (int32_t c = file_name_length - 1; c >= 0; --c)
        {
            if (absolute_file_name[c] == '/' || absolute_file_name[c] == '\\')
            {
                last_slash_pos = c;
                break;
            }
        }

        /// 并非绝对路径名
        if (last_slash_pos == 0)
        {
            return false;
        }
        else
        {
            std::memcpy(folder_name, absolute_file_name, last_slash_pos);
            folder_name[last_slash_pos] = 0;
            if (NativeDirectory::create_folder(folder_name) == false)
            {
                return false;
            }
        }
    }

    /// 尝试打开指定文件
    HANDLE exp_file_handle = INVALID_HANDLE_VALUE;

#if defined(UNICODE)
    std::wstring wchar_file_name;
    if (WCharHelper::utf8_to_wchar(absolute_file_name, wchar_file_name) == false)
    {
        return false;
    }
    const wchar_t * const absolute_file_name_used = wchar_file_name.c_str();

#else
    const char * const absolute_file_name_used = absolute_file_name;
#endif /// defined(UNICODE)

    switch (access_mode)
    {
        case AccessMode::READ_ONLY_ACCESS_MODE:
        {
            /// 如果文件不存在返回INVALID_HANDLE_VALUE
            exp_file_handle = CreateFile(
                absolute_file_name_used, GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            break;
        }

        case AccessMode::WRITE_ONLY_ACCESS_MODE:
        {
            /// 如果文件不存, 创建它
            exp_file_handle = CreateFile(
                absolute_file_name_used, GENERIC_WRITE,
                FILE_SHARE_READ,
                nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            break;
        }

        case AccessMode::READ_WRITE_ACCESS_MODE:
        {
            /// 如果文件不存在返回INVALID_HANDLE_VALUE
            exp_file_handle = CreateFile(
                absolute_file_name_used, GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ,
                nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            break;
        }

        default:
        {
            RUNTIME_ASSERT(false, "Unknown access mode!!");
            return false;
        }
    }

    /// 成功打开指定文件
    if (exp_file_handle != INVALID_HANDLE_VALUE)
    {
        /// 关闭当前打开的文件
        close();

        m_file_handle    = new WinFileHandle;
        REAL_FILE_HANDLE = exp_file_handle;
        /// 获取文件长度
        LARGE_INTEGER found_file_length;
        /// 返回非零, 如果成功
        if (GetFileSizeEx(REAL_FILE_HANDLE, &found_file_length))
        {
            m_file_length = (uint32_t)found_file_length.LowPart;
        }
        else
        {
            return false;
        }
        m_cursor_pos     = 0;
        m_is_file_opened = true;
        return true;
    }
    /// 无法打开指定文件
    else
    {
        return false;
    }
}


bool
NativeFile::is_opened () const
{
    return m_is_file_opened;
}


uint32_t
NativeFile::cursor_position () const
{
    return m_cursor_pos;
}


uint32_t
NativeFile::file_length () const
{
    return m_file_length;
}


bool
NativeFile::seek (
    const int32_t  cursor_pos,
    const SeekMode seek_mode)
{
    RUNTIME_ASSERT(m_is_file_opened, "The file is not opened!!");

    if (m_is_file_opened)
    {
        /// 用来保存Cursor相对于文件头的位置
        uint32_t absolute_cursor_pos;

        switch (seek_mode)
        {
            /// 相对于文件头部
            case SeekMode::FILE_BEGIN_SEEK_MODE:
            {
                /// 无法将读写头设置到文件头之前
                if (cursor_pos < 0)
                {
                    return false;
                }
                else
                {
                    absolute_cursor_pos = (uint32_t)cursor_pos;
                    break;
                }
            }

            /// 相对于当前位置
            case SeekMode::CURRENT_CURSOR_SEEK_MODE:
            {
                LARGE_INTEGER file_cursor_pos;

                /// 由于Windows没有提供GetFilePointer()函数，所以我们只能使用SetFilePointer()
                /// 返回非零, 如果成功
                if (SetFilePointerEx(REAL_FILE_HANDLE, LARGE_INTEGER{ 0 },
                                     &file_cursor_pos, FILE_CURRENT))
                {
                    absolute_cursor_pos =
                        (uint32_t)((int32_t)file_cursor_pos.LowPart + cursor_pos);
                    break;
                }
                else
                {
                    return false;
                }
            }

            /// 相对于文件尾
            case SeekMode::FILE_END_SEEK_MODE:
            {
                /// 无法将读写头设置到文件尾之后
                if (cursor_pos > 0)
                {
                    return false;
                }
                else
                {
                    /// 相对于文件尾计算期待位置
                    absolute_cursor_pos = (uint32_t)((int32_t)m_file_length + cursor_pos);
                    break;
                }
            }

            default:
            {
                RUNTIME_ASSERT(false, "Uknown seek mode!!");
                return false;
            }
        }
        RUNTIME_ASSERT(absolute_cursor_pos <= m_file_length, "Cursor position is out of range!!");

        /// 移动Cursor
        /// 返回非零, 如果成功
        if (SetFilePointerEx(REAL_FILE_HANDLE, LARGE_INTEGER{(DWORD)absolute_cursor_pos},
                             nullptr, FILE_BEGIN))
        {
            /// 更新读写头位置
            m_cursor_pos = absolute_cursor_pos;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


int32_t
NativeFile::read (
    uint8_t * const buffer_start,
    const uint32_t  buffer_size,
    const uint32_t  buffer_offset,
    const uint32_t  read_count)
{
    RUNTIME_ASSERT(buffer_start, "Output buffer can not be NULL!!");
    RUNTIME_ASSERT(buffer_offset + read_count <= buffer_size,
                   "No place to store all read bytes!!");
    RUNTIME_ASSERT(m_is_file_opened, "The file is not opened!!");

    /// 可以保存足够的数据, 并且文件够大
    if (buffer_start      && (buffer_offset + read_count <= buffer_size) &&
        m_is_file_opened  && (m_cursor_pos  + read_count <= m_file_length))
    {
        /// 同步读人数据
        /// 返回非零, 如果成功
        if (ReadFile(REAL_FILE_HANDLE, buffer_start + buffer_offset, read_count,
                     nullptr, nullptr))
        {
            /// 更新读写头位置
            m_cursor_pos += read_count;
            return read_count;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}


bool
NativeFile::write (
    const uint8_t * const buffer_start,
    const uint32_t        buffer_size,
    const uint32_t        buffer_offset,
    const uint32_t        write_count)
{
    RUNTIME_ASSERT(buffer_start, "Input buffer can not be NULL!!");
    RUNTIME_ASSERT(buffer_offset + write_count <= buffer_size,
                   "No enough data in the input buffer!!");
    RUNTIME_ASSERT(m_is_file_opened, "The file is not opened!!");

    /// 可以写出足够的数据
    if (buffer_start && (buffer_offset + write_count <= buffer_size) && m_is_file_opened)
    {
        /// 返回非零, 如果成功
        if (WriteFile(REAL_FILE_HANDLE, buffer_start + buffer_offset, write_count,
                      nullptr, nullptr))
        {
            /// 更新读写头位置
            m_cursor_pos  += write_count;
            /// 更新文件长度
            m_file_length += write_count;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


void
NativeFile::close ()
{
    /// 检查是否当前文件已经关闭
    if (m_is_file_opened)
    {
        /// 关闭文件并清除Handle
        CloseHandle(REAL_FILE_HANDLE);
        REAL_FILE_HANDLE = INVALID_HANDLE_VALUE;
        delete m_file_handle;

        m_file_handle    = nullptr;
        m_file_length    = 0;
        m_cursor_pos     = 0;
        m_is_file_opened = false;
    }
}

#endif /// (OS_TYPE == OS_TYPE_WIN)
