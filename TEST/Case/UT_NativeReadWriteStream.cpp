#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (NATIVE_READ_WRITE_STREAM_CHECKING == 1)
/// System headers
#include <cstdio>   /// std::printf, std::snprintf
#include <cstring>  /// std::strlen
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "FileSystem/NativeDirectory.hpp"
#include "FileSystem/NativeReadStream.hpp"
#include "FileSystem/NativeWriteStream.hpp"


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking Native ReadStream/WriteStream", "[Native ReadStream/WriteStream]")
{

    char file_name[1024];
    std::snprintf(file_name, sizeof(file_name),
                  "%s/UNIT_TEST/ReadWriteStream/ReadOnly_WriteOnlyStream.dat",
                  NativeDirectory::document_folder());

    //// 各种类型数据
    const int8_t       si8data_o   = -127;
    const uint8_t      ui8data_o   = 127;
    const char         asciidata_o = 'A';
    const int16_t      si16data_o  = -640;
    const uint16_t     ui16data_o  = 640;
    const int32_t      si32data_o  = -1640;
    const uint32_t     ui32data_o  = 1640;
    const int64_t      si64data_o  = -2640;
    const uint64_t     ui64data_o  = 2640;
    const float        r32data_o   = 2.34f;
    const double       r64data_o   = 4.56;
    const char         strdata_o[] = "SOMETHING+OTHERTHING";

    SECTION("Checking Native WriteStream")
    {
        std::printf("--- Checking Native WriteStream...\n");

        uint32_t file_length = 0;

        NativeWriteStream output_stream(file_name);
        REQUIRE((output_stream.m_is_file_opened && output_stream.is_opened()));

        REQUIRE((output_stream.file_length() == 0));
        output_stream << si8data_o  << ui8data_o  << asciidata_o;
        uint32_t write_size = sizeof(si8data_o) + sizeof(ui8data_o) + sizeof(asciidata_o);
        REQUIRE((output_stream.file_length() == file_length + write_size));
        file_length += write_size;

        output_stream << si16data_o << ui16data_o;
        write_size = sizeof(si16data_o) + sizeof(ui16data_o);
        REQUIRE((output_stream.file_length() == file_length + write_size));
        file_length += write_size;

        output_stream << si32data_o << ui32data_o;
        write_size = sizeof(si32data_o) + sizeof(ui32data_o);
        REQUIRE((output_stream.file_length() == file_length + write_size));
        file_length += write_size;

        output_stream << si64data_o << ui64data_o;
        write_size = sizeof(si64data_o) + sizeof(ui64data_o);
        REQUIRE((output_stream.file_length() == file_length + write_size));
        file_length += write_size;

        output_stream << r32data_o  << r64data_o;
        write_size = sizeof(r32data_o) + sizeof(r64data_o);
        REQUIRE((output_stream.file_length() == file_length + write_size));
        file_length += write_size;

        output_stream << strdata_o;
        write_size = (uint32_t)sizeof(strdata_o) - 1;
        REQUIRE((output_stream.file_length() == file_length + write_size));
        file_length += write_size;

        output_stream << LINE_FEED;
        write_size = (uint32_t)std::strlen(LINE_FEED);
        REQUIRE((output_stream.file_length() == file_length + write_size));
        file_length += write_size;

        output_stream.close();
        REQUIRE((NativeFile::does_file_exist(file_name)));
        REQUIRE((NativeFile::file_length(file_name) == file_length));

        std::printf("--- Checking Native WriteStream: OK!\n");
    } /// SECTION("Checking Native WriteStream")


    SECTION("Checking Native ReadStream")
    {
        std::printf("--- Checking Native ReadStream...\n");

        /// 输入各种类型数据
        int8_t   si8data_i;
        uint8_t  ui8data_i;
        char     asciidata_i;
        int16_t  si16data_i;
        uint16_t ui16data_i;
        int32_t  si32data_i;
        uint32_t ui32data_i;
        int64_t  si64data_i;
        uint64_t ui64data_i;
        float    r32data_i;
        double   r64data_i;
        char     strdata_i[sizeof(strdata_o)] = { 0 };

        uint32_t cursor_pos = 0;

        NativeReadStream  input_stream(file_name);
        REQUIRE((input_stream.m_is_file_opened && input_stream.is_opened()));

        input_stream >> si8data_i >> ui8data_i >> asciidata_i;
        uint32_t read_size = sizeof(si8data_i) + sizeof(ui8data_i) + sizeof(asciidata_i);
        REQUIRE((input_stream.cursor_position() == cursor_pos + read_size));
        cursor_pos += read_size;

        input_stream >> si16data_i >> ui16data_i;
        read_size = sizeof(si16data_i) + sizeof(ui16data_i);
        REQUIRE((input_stream.cursor_position() == cursor_pos + read_size));
        cursor_pos += read_size;

        input_stream >> si32data_i >> ui32data_i;
        read_size = sizeof(si32data_i) + sizeof(ui32data_i);
        REQUIRE((input_stream.cursor_position() == cursor_pos + read_size));
        cursor_pos += read_size;

        input_stream >> si64data_i >> ui64data_i;
        read_size = sizeof(si64data_i) + sizeof(ui64data_i);
        REQUIRE((input_stream.cursor_position() == cursor_pos + read_size));
        cursor_pos += read_size;

        input_stream >> r32data_i >> r64data_i;
        read_size = sizeof(r32data_i) + sizeof(r64data_i);
        REQUIRE((input_stream.cursor_position() == cursor_pos + read_size));
        cursor_pos += read_size;

        for (uint32_t c = 0; c < (uint32_t)sizeof(strdata_o) - 1; ++c)
        {
            input_stream >> strdata_i[c];
        }
        read_size = sizeof(strdata_o) - 1;
        REQUIRE((input_stream.cursor_position() == cursor_pos + read_size));
        cursor_pos += read_size;

        /// 检查数据
        REQUIRE((si8data_o   == si8data_i   ));
        REQUIRE((ui8data_o   == ui8data_i   ));
        REQUIRE((asciidata_o == asciidata_i ));
        REQUIRE((si16data_o  == si16data_i  ));
        REQUIRE((ui16data_o  == ui16data_i  ));
        REQUIRE((si32data_o  == si32data_i  ));
        REQUIRE((ui32data_o  == ui32data_i  ));
        REQUIRE((si64data_o  == si64data_i  ));
        REQUIRE((ui64data_o  == ui64data_i  ));
        REQUIRE((r32data_o   == r32data_i   ));
        REQUIRE((r64data_o   == r64data_i   ));
        REQUIRE((std::strcmp(strdata_o, strdata_i) == 0));

        std::printf("--- Checking Native ReadStream: OK\n");
    } /// SECTION("Checking Native ReadStream")


    SECTION("Checking File delete")
    {
        std::printf("--- Checking File delete...\n");

        REQUIRE((NativeFile::delete_file(file_name)));
        REQUIRE((!NativeFile::does_file_exist(file_name)));

        std::printf("--- Checking File delete: OK\n");
    } /// SECTION("Checking File delete")

} /// TEST_CASE("Checking Native ReadStream/WriteStream", "[Native ReadStream/WriteStream]")

#endif /// (NATIVE_READ_WRITE_STREAM_CHECKING == 1)
