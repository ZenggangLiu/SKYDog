/// System headers
#include <algorithm> /// std::min
#include <cstring>   /// std::memcpy
/// Library headers
#include "Assert/RuntimeAssert.hpp"
/// Self header
#include "Encoder/X2TeaEncoder.hpp"


/// XXTEA cipher: https://en.wikipedia.org/wiki/XXTEA
///
/// Key的长度(128位Key): 16字节
#define KEY_LENGTH  16
#define COPY_KEY(key, key_length) \
    uint32_t _key_used[KEY_LENGTH >> 2] = {0}; std::memcpy(_key_used, key, std::min((uint32_t)key_length, (uint32_t)sizeof(_key_used)));
#define DELTA 0x9E3779B9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + ((_key_used)[(p&3)^e] ^ z)))


void
XXTEA::encode (
    const uint8_t *  const key,
    const uint32_t   key_length,
    uint32_t * const data_ptr,
    const uint32_t   data_size)
{
    RUNTIME_ASSERT(data_size >= 2, "Data is too short. It is at least a data of two 32bits integers!!");

    /// 由于XXTEA使用128位Key，所以此处进行Key的生成
    COPY_KEY(key, key_length);

    /// 编码data_ptr中的数据
    uint32_t y, z, sum, p, rounds, e;
    rounds = 6 + 52/data_size;
    sum    = 0;
    z      = data_ptr[data_size-1];
    do
    {
        sum += DELTA;
        e    = (sum >> 2) & 3;
        for (p = 0; p < data_size-1; p++)
        {
            y = data_ptr[p+1];
            z = data_ptr[p] += MX;
        }
        y = data_ptr[0];
        z = data_ptr[data_size-1] += MX;
    } while (--rounds);
}


void
XXTEA::decode (
    const uint8_t *  const key,
    const uint32_t   key_length,
    uint32_t * const data_ptr,
    const uint32_t   data_size)
{
    RUNTIME_ASSERT(data_size >= 2, "Data is too short. At least, it is a two 32bits integers!!");

    /// 由于XXTEA使用128位Key，所以此处进行Key的生成
    COPY_KEY(key, key_length);

    /// 解码data_ptr中的数据
    uint32_t y, z, sum, p, rounds, e;
    rounds = 6 + 52/data_size;
    sum    = rounds*DELTA;
    y      = data_ptr[0];
    do
    {
        e = (sum >> 2) & 3;
        for (p = data_size - 1; p > 0; p--)
        {
            z = data_ptr[p - 1];
            y = data_ptr[p] -= MX;
        }
        z    = data_ptr[data_size - 1];
        y    = data_ptr[0] -= MX;
        sum -= DELTA;
    } while (--rounds);
}
