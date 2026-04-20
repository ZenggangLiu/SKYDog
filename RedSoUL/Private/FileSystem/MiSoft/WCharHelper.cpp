#include "Common/PlatformDefines.hpp" /// OS_TYPE
#if (OS_TYPE == OS_TYPE_WIN) && defined(UNICODE)

/// System headers
#include <Windows.h>
/// Self header
#include "FileSystem/MiSoft/WCharHelper.hpp"


bool
WCharHelper::utf8_to_wchar (
    const char * const utf8_name,
    std::wstring &     conved_name)
{
    /// -1代表: 自动计算长度(包含 L'\0')
    const int32_t wchar_count =(int32_t)MultiByteToWideChar(
        CP_UTF8, 0, utf8_name, -1, nullptr, 0);

    conved_name.resize(wchar_count);

    /// 返回非零, 如果成功
    const bool op_code = MultiByteToWideChar(
        CP_UTF8, 0, utf8_name, -1, (wchar_t*)conved_name.data(), wchar_count);

    return op_code;
}


bool
WCharHelper::wchar_to_utf8 (
    const wchar_t * const wchar_name,
    std::string &         conved_name)
{
    /// -1代表: 自动计算长度(包含 '\0')
    int32_t utf8_count = (int32_t)WideCharToMultiByte(
        CP_UTF8, 0, wchar_name, -1, nullptr, 0,
        nullptr, nullptr);

    conved_name.resize(utf8_count);

    /// 返回非零, 如果成功
    const bool op_code = WideCharToMultiByte(
        CP_UTF8, 0, wchar_name, -1, (char*)conved_name.data(), utf8_count,
        nullptr, nullptr);

    return op_code;
}

#endif /// (OS_TYPE == OS_TYPE_WIN) && defined(UNICODE)
