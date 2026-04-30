#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (NATIVE_DIRECTORY_CHECKING == 1)
/// System headers
#include <cstdio> /// std::snprintf
/// Library headers
#include "FileSystem/NativeDirectory.hpp"


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking Native Directory", "[Native Directory]")
{

    SECTION("Checking Native Directory")
    {
        std::printf("--- Checking Native Directory...\n");

        std::printf("[Cache Folder]:     %s\n", NativeDirectory::cache_folder());
        std::printf("[Document Folder]:  %s\n", NativeDirectory::document_folder());
        std::printf("[Temporary Folder]: %s\n", NativeDirectory::temporary_folder());
        std::printf("[Working Folder]:   %s\n", NativeDirectory::working_folder());

        char unix_path[1024];
        char win_path[1024];
        /// 編: 0xE7, 0xB7, 0xA8
        /// 碼: 0xE7, 0xA2, 0xBC
        /// 测: 0xE6, 0xB5, 0x8B
        /// 试: 0xE8, 0xAF, 0x95
        ///
        std::snprintf(unix_path, sizeof(unix_path),
                      "%s/UNIT_TEST/FileSystem/%s",
                      NativeDirectory::document_folder(), u8"編碼测试");
        std::snprintf(win_path, sizeof(unix_path),
                      "%s\\UNIT_TEST\\FileSystem/%s",
                      NativeDirectory::document_folder(), u8"編碼测试");

        /// 检测使用两种分隔符创建目录
        /// NOTE: 在Windows下可以成功使用Unix分隔符
        REQUIRE((NativeDirectory::create_folder(unix_path)));
        REQUIRE((NativeDirectory::does_folder_exist(unix_path)));
        REQUIRE((NativeDirectory::delete_folder(unix_path)));
        REQUIRE((!NativeDirectory::does_folder_exist(unix_path)));

        REQUIRE((NativeDirectory::create_folder(win_path)));
        REQUIRE((NativeDirectory::does_folder_exist(win_path)));
        REQUIRE((NativeDirectory::delete_folder(win_path)));
        REQUIRE((!NativeDirectory::does_folder_exist(win_path)));

        std::printf("--- Checking Native Directory: OK!\n");
    } /// SECTION("Checking Native Directory")

} /// TEST_CASE("Checking Native Directory", "[Native Directory]")

#endif /// (NATIVE_DIRECTORY_CHECKING == 1)
