#include "Common/PlatformDefines.hpp" /// OS_TYPE
#if (OS_TYPE == OS_TYPE_WIN) && defined(UNICODE)

/// System headers
#include <string>


struct WCharHelper
{
    /// UTF8 ==> WCHAR
    static
    bool
    utf8_to_wchar (
        const char * const utf8_name,
        std::wstring &     conved_name);

    /// WCHAR ==> UTF8
    static
    bool
    wchar_to_utf8 (
        const wchar_t * const wchar_name,
        std::string &         conved_name);
};

#endif /// (OS_TYPE == OS_TYPE_WIN) && defined(UNICODE)
