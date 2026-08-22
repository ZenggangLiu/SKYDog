#include "Common/PlatformDefines.hpp" /// OS_TYPE 
/// System headers
#if (OS_TYPE == OS_TYPE_WIN)
#include <stdlib.h> /// _byteswap_ushort/_ulong/_uint64()
#endif
/// Self header
#include "Math/EndianUtilities.hpp"


bool
EndianUtility::is_big_endian ()
{
    /// 初始化一个uint32_t为: 0x04030201
    /// 低位-->高位的排列:  LSB            MSB
    /// - Big    Endian: 0x04 0x03 0x02 0x01
    /// - Little Endian: 0x01 0x02 0x03 0x04
    static constexpr
    union
    {
        uint32_t ui32;
        uint8_t  ui8[4];
    } ENDIAN_CHECKING_DATA = { 0x04030201 };
    /// 是否为: 0x04 0x03 0x02 0x01
    return ENDIAN_CHECKING_DATA.ui8[0] == 0x04;
}


uint16_t
EndianUtility::swap_endian_16 (
    const uint16_t data)
{
#if (OS_TYPE == OS_TYPE_WIN)
    return (uint16_t)_byteswap_ushort((const unsigned short)data);
#else
    return __builtin_bswap16(data);
#endif
}


uint32_t
EndianUtility::swap_endian_32 (
    const uint32_t data)
{
#if (OS_TYPE == OS_TYPE_WIN)
    return (uint32_t)_byteswap_ulong((const unsigned long)data);
#else
    return __builtin_bswap32(data);
#endif
}


uint64_t
EndianUtility::swap_endian_64 (
    const uint64_t data)
{
#if (OS_TYPE == OS_TYPE_WIN)
    return (uint64_t)_byteswap_uint64((const __int64)data);
#else
    return __builtin_bswap64(data);
#endif
}
