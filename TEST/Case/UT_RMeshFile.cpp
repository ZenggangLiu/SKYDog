#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (RENDER_MESH_FILE_CHECKING == 1)
/// System headers
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "FileSystem/NativeDirectory.hpp"
#include "FileSystem/NativeFile.hpp"
#include "IO/MeshFile/RMeshFile.hpp"
#include "Render/RenderMeshId.hpp"
#include "Render/RenderMeshDepot.hpp"


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking RenderMesh File", "[RenderMesh File]")
{

    SECTION("Checking RenderMesh File")
    {
        std::printf("--- Checking RenderMesh File...\n");

        char file_name[1024];
        std::snprintf(file_name, sizeof(file_name),
                      "%s/UNIT_TEST/RMeshFile/cube_mesh_file.rmesh",
                      NativeDirectory::document_folder());

        const RenderMeshIdT cube_id = RenderMeshDepot::ref().create_unit_cube();
        RenderMeshDepot::ref().write_to_mesh_file(file_name, cube_id);
        REQUIRE((NativeFile::does_file_exist(file_name)));

        RenderMeshDepot::ref().clear();

        const RenderMeshIdT loaded_cube_id =
            RenderMeshDepot::ref().create_from_mesh_file(file_name, cube_id);
        REQUIRE((cube_id == loaded_cube_id));
        RenderMeshDepot::ref().write_to_ply_file(file_name, loaded_cube_id);

        std::printf("--- Checking RenderMesh File: OK!\n");
    }

} /// TEST_CASE("Checking RenderMesh File", "[RenderMesh File]")

#endif /// (RENDER_MESH_FILE_CHECKING == 1)
