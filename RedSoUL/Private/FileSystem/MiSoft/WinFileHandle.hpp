#include "Common/PlatformDefines.hpp" /// OS_TYPE
#if (OS_TYPE == OS_TYPE_WIN)

/// System headers
#include <Windows.h> /// HANDLE
/// Library header
#include "FileSystem/FileHandle.hpp"


struct WinFileHandle : FileHandle
{
    HANDLE real_handle;
};

#endif /// (OS_TYPE == OS_TYPE_WIN)
