#include "Common/PlatformDefines.hpp" /// OS_TYPE 
/// System headers
#if (OS_TYPE == OS_TYPE_WIN)
#include <intrin.h> /// _BitScanForward64
#endif
/// Self header
#include "Math/BitOperationUtilities.hpp"


uint8_t
BitOperationUtility::bit_scan_forward64 (
    const uint64_t data)
{
    if (data)
    {
#if (OS_TYPE == OS_TYPE_WIN)
        unsigned long setbit_pos;

        /// 如果数值为0, _BitScanForward64()返回0
        if (_BitScanForward64(&setbit_pos, data))
        {
            return (uint8_t)setbit_pos;
        }
        else
        {
            return NO_SETBIT_AT_ALL;
        }
#else
        /// __builtin_ctzll()返回从最低位开始多少个连续的0
        return (uint8_t)__builtin_ctzll(data);
#endif
    }
    else
    {
        return NO_SETBIT_AT_ALL;
    }
}
