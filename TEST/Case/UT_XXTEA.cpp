#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (XXTEA_CODEC_CHECKING == 1)
/// Library headers
#include "Codec/X2TeaCodec.hpp"


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking XXTEA encode/decode", "[XXTEA]")
{

    const uint8_t key[] = { 0, 1, 2, 3, 4, 5 };
    const uint8_t clear_txt[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    const uint8_t encoded_txt[] =
    {
        0x50, 0x23, 0x8A, 0xAF,
        0x5C, 0xF7, 0x59, 0x71,
        0xCD, 0xA8, 0x1A, 0x12,
        0x56, 0xAD, 0x01, 0x1B
    };
    /// 操作用的缓存
    uint32_t working_buffer[sizeof(clear_txt) >> 2];


    SECTION("Checking Encoding:")
    {
        std::printf("--- Checking XXTEA Encoding...\n");

        std::memcpy(working_buffer, clear_txt, sizeof(working_buffer));
        XXTEA_ENCODE(key, sizeof(key), working_buffer);
        for (uint32_t c = 0; c < sizeof(working_buffer); ++c)
        {
            REQUIRE((((const uint8_t*)working_buffer)[c] == encoded_txt[c]));
        }

        std::printf("--- Checking XXTEA Encoding: OK!\n");
    }

    SECTION("Checking Decoding:")
    {
        std::printf("--- Checking XXTEA Decoding...\n");

        std::memcpy(working_buffer, encoded_txt, sizeof(working_buffer));
        XXTEA_DECODE(key, sizeof(key), working_buffer);
        for (uint32_t c = 0; c < sizeof(working_buffer); ++c)
        {
            REQUIRE((((const uint8_t*)working_buffer)[c] == clear_txt[c]));
        }

        std::printf("--- Checking XXTEA Decoding: OK!\n");
    }

} /// TEST_CASE("Checking XXTEA encode/decode", "[XXTEA]")

#endif /// (XXTEA_CODEC_CHECKING == 1)
