#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (EXR_IMAGE_FILE_CHECKING == 1)
/// System headers
#include <cstdio>   /// std::printf, std::snprintf
#include <stdint.h> /// uint32_t,...
#include <vector>
/// Library headers
#include "DataType/HdrColor.hpp"
#include "FileSystem/NativeDirectory.hpp"
#include "FileSystem/NativeFile.hpp"
#include "IO/ImageFile/ExrFile.hpp"


struct CheckerBoardCreateParam
{
    uint32_t image_width;
    uint32_t image_height;
    uint32_t cell_texel_size;
    uint32_t cell_border_texel_width;
    HdrColor cell_color_0;
    HdrColor cell_color_1;
    HdrColor cell_border_color;
};


static
bool
is_cell_border_texel (
    const uint32_t texel_pos,
    const uint32_t image_texel_count,
    const uint32_t cell_texel_size,
    const uint32_t cell_border_texel_width)
{
    if (cell_border_texel_width > 0)
    {
        if (cell_border_texel_width >= cell_texel_size ||
            cell_border_texel_width >= image_texel_count)
        {
            return true;
        }
        else
        {
            const uint32_t local_pos = texel_pos % cell_texel_size;
            if (local_pos < cell_border_texel_width)
            {
                return true;
            }
            else
            {
                return texel_pos >= image_texel_count - cell_border_texel_width;
            }
        }
    }
    else
    {
        return false;
    }
}


static
void
generate_checker_board (
    const CheckerBoardCreateParam & param,
    std::vector<HdrColor> &         image_data)
{
    REQUIRE((param.image_width > 0));
    REQUIRE((param.image_height > 0));
    REQUIRE((param.cell_texel_size > 0));

    image_data.resize(param.image_width * param.image_height);

    for (uint32_t y = 0; y < param.image_height; ++y)
    {
        for (uint32_t x = 0; x < param.image_width; ++x)
        {
            const bool is_border =
                is_cell_border_texel(x, param.image_width,
                                     param.cell_texel_size,
                                     param.cell_border_texel_width) ||
                is_cell_border_texel(y, param.image_height,
                                     param.cell_texel_size,
                                     param.cell_border_texel_width);

            const uint32_t data_idx = y * param.image_width + x;
            if (is_border)
            {
                image_data[data_idx] = param.cell_border_color;
            }
            else
            {
                const uint32_t cell_x = x / param.cell_texel_size;
                const uint32_t cell_y = y / param.cell_texel_size;
                const bool use_cell_color_0 = ((cell_x + cell_y) & 1) == 0;
                image_data[data_idx] =
                    use_cell_color_0 ? param.cell_color_0 : param.cell_color_1;
            }
        }
    }
}


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking EXR File", "[EXR File]")
{

    SECTION("Checking EXR checker board")
    {
        std::printf("--- Checking EXR File...\n");

        CheckerBoardCreateParam checker_board_param =
        {
            512, /// image width(texels)
            521, /// image height(texels)
            256, /// cell size(texels)
            12,  /// cell border width(texels)
            HdrColor::make(0.3419f, 0.4020f, 0.5271f, 1.0f), /// color of cell0
            HdrColor::make(0.3419f, 0.4020f, 0.5271f, 1.0f), /// color of cell1
            HdrColor::make(1.0000f, 1.0000f, 1.0000f, 1.0f), /// board color
        };

        std::vector<HdrColor> image_data;
        generate_checker_board(checker_board_param, image_data);
        REQUIRE((image_data.size() ==
                 checker_board_param.image_width * checker_board_param.image_height));

        char file_name[1024];
        std::snprintf(
            file_name, sizeof(file_name),
            "%s/UNIT_TEST/ExrFile/checker_board_%ux%u_cell%u_border%u.exr",
            NativeDirectory::document_folder(),
            checker_board_param.image_width,
            checker_board_param.image_height,
            checker_board_param.cell_texel_size,
            checker_board_param.cell_border_texel_width);

        REQUIRE((ExrFile::write_to(
                 file_name,
                 checker_board_param.image_width,
                 checker_board_param.image_height,
                 image_data.data(),
                 (uint32_t)image_data.size())));
        REQUIRE((NativeFile::does_file_exist(file_name)));
        REQUIRE((NativeFile::file_length(file_name) > 0));

        std::printf("[Output Exr]: %s\n", file_name);
        std::printf("--- Checking EXR File: OK!\n");
    }

} /// TEST_CASE("Checking EXR File", "[EXR File]")

#endif /// (EXR_IMAGE_FILE_CHECKING == 1)
