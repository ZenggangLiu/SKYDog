#include "Common/PlatformDefines.hpp" /// OS_TYPE
#if (OS_TYPE == OS_TYPE_WIN)

/// System headers
#include <string>     /// std::wstring
#include <Shlobj.h>   /// SHGetFolderPath()
#include <tchar.h>    /// _tcscmp
#include <Windows.h>
/// Library headers
#include "Assert/RuntimeAssert.hpp"
#include "Common/CommonDefines.hpp" /// ARRAY_SIZE
#include "FileSystem/MiSoft/WCharHelper.hpp"
/// Self header
#include "FileSystem/NativeDirectory.hpp"


static
std::string
fetch_cache_folder_name ()
{
    TCHAR folder_name[MAX_PATH + sizeof(TCHAR)] = { 0 };

    /// "C:\Users\User_Name\AppData\Local"
    SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, folder_name);
    /// "C:\Users\User_Name\AppData\Roaming"
    /// SHGetFolderPath(0, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, _folder_name);

#if defined(UNICODE)
    std::string utf8_folder_name;
    WCharHelper::wchar_to_utf8(folder_name, utf8_folder_name);
    return utf8_folder_name;

#else
    return std::string(folder_name);
#endif
}


static
std::string
fetch_document_folder_name ()
{
    TCHAR folder_name[MAX_PATH + sizeof(TCHAR)] = { 0 };

    /// "C:\Users\User_Name\Documents"
    SHGetFolderPath(0, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, folder_name);

#if defined(UNICODE)
    std::string utf8_folder_name;
    WCharHelper::wchar_to_utf8(folder_name, utf8_folder_name);
    return utf8_folder_name;

#else
    return std::string(folder_name);
#endif
}


static
std::string
fetch_temporary_folder_name ()
{
    TCHAR folder_name[MAX_PATH + sizeof(TCHAR)] = { 0 };

    /// "C:\Users\User_Name\AppData\Local\Temp\"
    GetTempPath(ARRAY_SIZE(folder_name), folder_name);

#if defined(UNICODE)
    std::string utf8_folder_name;
    WCharHelper::wchar_to_utf8(folder_name, utf8_folder_name);
    return utf8_folder_name;

#else
    return std::string(folder_name);
#endif
}


static
std::string
fetch_working_folder_name ()
{
    TCHAR folder_name[MAX_PATH + sizeof(TCHAR)] = { 0 };

    GetCurrentDirectory(ARRAY_SIZE(folder_name), folder_name);

#if defined(UNICODE)
    std::string utf8_folder_name;
    WCharHelper::wchar_to_utf8(folder_name, utf8_folder_name);
    return utf8_folder_name;

#else
    return std::string(folder_name);
#endif
}


/// 调用系统API创建指定目录
static
bool
create_named_folder (
    const char * const absolute_folder_name)
{
#if defined(UNICODE)
    std::wstring wchar_folder_name;
    if (WCharHelper::utf8_to_wchar(absolute_folder_name, wchar_folder_name) == false)
    {
        return false;
    }

    /// 返回非零, 如果成功
    if (CreateDirectory(wchar_folder_name.c_str(), nullptr))

#else
    if (CreateDirectory(absolute_folder_name, nullptr))
#endif /// defined(UNICODE)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/// 调用系统API递归删除指定目录
static
bool
delete_named_folder_recur (
    const TCHAR * const absolute_folder_name)
{
    /// 创建search pattern: "folder_name\*"
    TCHAR search_pattern[MAX_PATH + sizeof(TCHAR)];
    _stprintf_s(
        search_pattern, ARRAY_SIZE(search_pattern),
        TEXT("%s\\*"), absolute_folder_name);

    /// 查看指定目录是否存在, 以及有文件或子目录
    /// file_info表示File以及Folder
    WIN32_FIND_DATA file_info;
    const HANDLE file_handle = FindFirstFile(search_pattern, &file_info);

    /// 找不到目录
    if (file_handle == INVALID_HANDLE_VALUE)
    {
        /// const DWORD error_code = GetLastError();
        return false;
    }
    else
    {
        bool opcode = true;
        do
        {
            /// 为'.'或者'..'子目录
            if (_tcscmp(file_info.cFileName, TEXT(".") ) == 0 ||
                _tcscmp(file_info.cFileName, TEXT("..")) == 0)
            {
                continue;
            }

            /// 使用当前路径/文件名更新Search Pattern: "folder_name\SOMETHING"
            _stprintf_s(
                search_pattern, ARRAY_SIZE(search_pattern),
                TEXT("%s\\%s"), absolute_folder_name, file_info.cFileName);

            /// 子目录
            if (file_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                opcode = opcode && delete_named_folder_recur(search_pattern);
            }
            /// 文件
            else
            {
                /// 返回非零, 如果成功
                opcode = opcode && DeleteFile(search_pattern);
            }
        } while (FindNextFile(file_handle, &file_info));

        FindClose(file_handle);

        /// 返回非零, 如果成功
        return opcode && RemoveDirectory(absolute_folder_name);
    }
}



const char *
NativeDirectory::cache_folder ()
{
    static std::string CACHE_FOLDER_NAME = fetch_cache_folder_name();
    return CACHE_FOLDER_NAME.c_str();
}


const char *
NativeDirectory::document_folder ()
{
    static std::string DOCUMENT_FOLDER_NAME = fetch_document_folder_name();
    return DOCUMENT_FOLDER_NAME.c_str();
}


const char *
NativeDirectory::temporary_folder ()
{
    static std::string TEMPORARY_FOLDER_NAME = fetch_temporary_folder_name();
    return TEMPORARY_FOLDER_NAME.c_str();
}


const char *
NativeDirectory::working_folder ()
{
    static std::string WORKING_FOLDER_NAME = fetch_working_folder_name();
    return WORKING_FOLDER_NAME.c_str();
}


bool
NativeDirectory::does_folder_exist (
    const char * const absolute_folder_name)
{
    RUNTIME_ASSERT( absolute_folder_name, "Directory name can not be NULL!!");
    RUNTIME_ASSERT(*absolute_folder_name, "Directory name can not be empty!!");

#if defined(UNICODE)
    std::wstring wchar_folder_name;
    if (WCharHelper::utf8_to_wchar(absolute_folder_name, wchar_folder_name) == false)
    {
        return false;
    }

    /// 返回INVALID_FILE_ATTRIBUTES, 如果失败
    DWORD file_attr = GetFileAttributes(wchar_folder_name.c_str());

#else
    /// 返回INVALID_FILE_ATTRIBUTES, 如果失败
    DWORD file_attr = GetFileAttributes(absolute_folder_name);
#endif /// defined(UNICODE)

    if (file_attr == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }
    else
    {
        return file_attr & FILE_ATTRIBUTE_DIRECTORY;
    }
}


bool
NativeDirectory::set_working_folder (
    const char * const absolute_folder_name)
{
    RUNTIME_ASSERT( absolute_folder_name, "Path name can not be NULL!!");
    RUNTIME_ASSERT(*absolute_folder_name, "Path name can not be empty!!");

#if defined(UNICODE)
    std::wstring wchar_folder_name;
    if (WCharHelper::utf8_to_wchar(absolute_folder_name, wchar_folder_name) == false)
    {
        return false;
    }

    /// 返回非零, 如果成功
    return SetCurrentDirectory(wchar_folder_name.c_str());

#else
    return SetCurrentDirectory(absolute_folder_name);
#endif /// defined(UNICODE)
}


bool
NativeDirectory::create_folder (
    const char * const absolute_folder_name)
{
    RUNTIME_ASSERT( absolute_folder_name, "Path name can not be NULL!!");
    RUNTIME_ASSERT(*absolute_folder_name, "Path name can not be empty!!");

    /// 检查是否指定的目录存在
    if (does_folder_exist(absolute_folder_name))
    {
        return true;
    }

    /// 创建所有中间目录
    std::string current_folder;
    std::string folder_name_str(absolute_folder_name);
    size_t delimiter_pos = 0;

    /// '\' OR '/'
    while ((delimiter_pos = folder_name_str.find_first_of("\\/", delimiter_pos)) !=
           std::string::npos)
    {
        /// 为了检测"D:\"。因为如果检测"D:"，doesPathExist()将失败！
        current_folder = folder_name_str.substr(0, ++delimiter_pos);

        if (current_folder.empty() || does_folder_exist(current_folder.c_str()))
        {
            continue;
        }
        else
        {
            if (!create_named_folder(current_folder.c_str()))
            {
                return false;
            }
        }
    }

    /// 创建最上层目录
    return create_named_folder(absolute_folder_name);
}


bool
NativeDirectory::delete_folder (
    const char * const absolute_folder_name)
{
    RUNTIME_ASSERT( absolute_folder_name, "Path name can not be NULL!!");
    RUNTIME_ASSERT(*absolute_folder_name, "Path name can not be empty!!");

#if defined(UNICODE)
    std::wstring wchar_folder_name;
    if (WCharHelper::utf8_to_wchar(absolute_folder_name, wchar_folder_name))
    {
        return delete_named_folder_recur(wchar_folder_name.c_str());
    }
    else
    {
        return false;
    }

#else
    return delete_named_folder_recur(absolute_folder_name);
#endif /// defined(UNICODE)
}

#endif /// (OS_TYPE == OS_TYPE_WIN)
