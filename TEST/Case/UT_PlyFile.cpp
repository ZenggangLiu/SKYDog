#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (PLY_MESH_FILE_CHECKING == 1)
/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "FileSystem/NativeDirectory.hpp"
#include "IO/MeshFile/PlyFile.hpp"
#include "Render/RenderMeshId.hpp"
#include "Render/RenderMeshDepot.hpp"


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking PLY File", "[PLY File]")
{

    SECTION("Checking PLY File")
    {
        std::printf("--- Checking PLY File...\n");

        char file_name[1024];
        //std::snprintf(file_name, sizeof(file_name),
        //              "%s/UNIT_TEST/PlyFile/cube_mesh_file",
        //              NativeDirectory::document_folder());

        //const RenderMeshIdT cube_id = RenderMeshDepot::ref().create_unit_cube();
        //RenderMeshDepot::ref().write_to_ply_file(file_name, cube_id);

        std::snprintf(file_name, sizeof(file_name),
                      "%s/UNIT_TEST/PlyFile/beveled_cube",
                      NativeDirectory::document_folder());

        const RenderMeshIdT icosahedron_id = RenderMeshDepot::ref().create_unit_beveled_cube();
        RenderMeshDepot::ref().write_to_ply_file(file_name, icosahedron_id);

        std::printf("--- Checking PLY File: OK!\n");
    }

} /// TEST_CASE("Checking PLY File", "[PLY File]")

#endif /// (PLY_MESH_FILE_CHECKING == 1)
