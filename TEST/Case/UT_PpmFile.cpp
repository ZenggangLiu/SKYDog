#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (PPM_IMAGE_FILE_CHECKING == 1)
/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "DataType/LdrColor.hpp"
#include "FileSystem/NativeDirectory.hpp"
#include "IO/ImageFile/PpmFile.hpp"


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking PPM File", "[PPM File]")
{

    SECTION("Checking PPM File")
    {
        std::printf("--- Checking PPM File...\n");

        /// 工作缓存大小
        static constexpr uint8_t IMAGE_WIDTH  = 3;
        static constexpr uint8_t IMAGE_HEIGHT = 4;

        /// 色彩分布：
        ///   ^ UP
        ///   |
        ///   |
        ///   | BLACK     1/2*RED_GREEN_BLUE    RED_GREEN_BLUE
        ///   | BLUE      RED_BLUE              GREEN_BLUE
        ///   | GREEN     RED_GREEN             GREEN_BLUE
        ///   | RED       RED_GREEN             RED_BLUE
        /// O +-------------------------------------------------> RIGHT
        ///
        /// BGRA8原始字节布局
        const LdrColor IMAGE_DATA_BGRA8[] =
        {
            /// RED             RED_GREEN       RED_BLUE
            {0,   0,   255, 255}, {0,   255, 255, 255}, {255, 0,   255, 255},
            /// GREEN           RED_GREEN       GREEN_BLUE
            {0,   255, 0,   255}, {0,   255, 255, 255}, {255, 255, 0,   255},
            /// BLUE            RED_BLUE        GREEN_BLUE
            {255, 0,   0,   255}, {255, 0,   255, 255}, {255, 255, 0,   255},
            /// BLACK           1/2*RGB         RED_GREEN_BLUE
            {0,   0,   0,   255}, {127, 127, 127, 255}, {255, 255, 255, 255}
        };

        /// RGBA8
        const LdrColor IMAGE_DATA_RGBA8[] =
        {
            /// RED             RED_GREEN       RED_BLUE
            {255, 0,   0,   255}, {255, 255, 0,   255}, {255, 0,   255, 255},
            /// GREEN           RED_GREEN       GREEN_BLUE
            {0,   255, 0,   255}, {255, 255, 0,   255}, {0,   255, 255, 255},
            /// BLUE            RED_BLUE        GREEN_BLUE
            {0,   0,   255, 255}, {255, 0,   255, 255}, {0,   255, 255, 255},
            /// BLACK           1/2*RGB         RED_GREEN_BLUE
            {0,   0,   0,   255}, {127, 127, 127, 255}, {255, 255, 255, 255}
        };

        const uint32_t IMAGE_PIXEL_COUNT = IMAGE_WIDTH * IMAGE_HEIGHT;

        char file_name[1024];

        /// ### --- BGR8 --- ###
        /// BGR8 ASCII格式
        {
            std::snprintf(file_name, sizeof(file_name),
                          "%s/UNIT_TEST/PpmFile/BGR8_image_data_ASCII_file",
                          NativeDirectory::document_folder());

            REQUIRE((PpmFile::write_to(
                        file_name,
                        IMAGE_WIDTH, IMAGE_HEIGHT,
                        IMAGE_DATA_BGRA8, IMAGE_PIXEL_COUNT,
                        false,    /// is RGBA
                        true,     /// use Alpha
                        false))); /// use binary file
        }

        /// BGR8 BINARY格式
        {
            std::snprintf(file_name, sizeof(file_name),
                          "%s/UNIT_TEST/PpmFile/BGR8_image_data_BINARY_file.ppm",
                          NativeDirectory::document_folder());

            REQUIRE((PpmFile::write_to(
                        file_name,
                        IMAGE_WIDTH, IMAGE_HEIGHT,
                        IMAGE_DATA_BGRA8, IMAGE_PIXEL_COUNT,
                        false,   /// is RGBA
                        true,    /// use Alpha
                        true))); /// use binary file
        }

        /// RGB8 ASCII格式
        {
            std::snprintf(file_name, sizeof(file_name),
                          "%s/UNIT_TEST/PpmFile/RGB8_image_data_ASCII_file.SOMETHINGWRONG",
                          NativeDirectory::document_folder());

            REQUIRE((PpmFile::write_to(
                        file_name,
                        IMAGE_WIDTH, IMAGE_HEIGHT,
                        IMAGE_DATA_RGBA8, IMAGE_PIXEL_COUNT,
                        true,     /// is RGBA
                        true,     /// use Alpha
                        false))); /// use binary file
            std::printf("Output: %s\n", file_name);

        }

        /// RGB8 BINARY格式
        {
            std::snprintf(file_name, sizeof(file_name),
                          "%s/UNIT_TEST/PpmFile/RGB8_image_data_BINARY_file",
                          NativeDirectory::document_folder());

            REQUIRE((PpmFile::write_to(
                        file_name,
                        IMAGE_WIDTH, IMAGE_HEIGHT,
                        IMAGE_DATA_RGBA8, IMAGE_PIXEL_COUNT,
                        true,    /// is RGBA
                        true,    /// use Alpha
                        true))); /// use binary file
            std::printf("Output: %s\n", file_name);
         }

        std::printf("--- Checking PPM File: OK!\n");
    }

} /// TEST_CASE("Checking PPM File", "[PPM File]")

#endif /// (PPM_IMAGE_FILE_CHECKING == 1)
