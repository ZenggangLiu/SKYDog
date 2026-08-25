#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (RENDER_MESH_FILE_CHECKING == 1)
/// System headers
#include <cstdio>   /// std::printf, std::snprintf
#include <cstring>  /// std::memcmp, std::memcpy
#include <stdint.h> /// uint32_t,...
#include <vector>
/// Library headers
#include "FileSystem/NativeDirectory.hpp"
#include "FileSystem/NativeFile.hpp"
#include "IO/MeshFile/RMeshFile.hpp"
#include "Render/RenderMesh.hpp"
#include "Render/RenderMeshId.hpp"
#include "Render/RenderMeshDepot.hpp"


struct RenderMeshDataCopy
{
    RenderMeshIdT        mesh_id;
    AABB                 bound_box;
    uint32_t             vertex_count;
    uint32_t             triangle_count;
    uint16_t             vertex_layout;
    std::vector<uint8_t> vertex_list;
    std::vector<uint8_t> triangle_list;
};


static
void
copy_render_mesh_data (
    const RenderMesh &    mesh_data,
    RenderMeshDataCopy &  mesh_data_copy)
{
    REQUIRE((mesh_data.vertex_list));
    REQUIRE((mesh_data.triangle_list));
    REQUIRE((mesh_data.vertex_list_size > 0));
    REQUIRE((mesh_data.triangle_list_size > 0));

    mesh_data_copy.mesh_id        = mesh_data.mesh_id;
    mesh_data_copy.bound_box      = mesh_data.bound_box;
    mesh_data_copy.vertex_count   = mesh_data.vertex_count;
    mesh_data_copy.triangle_count = mesh_data.triangle_count;
    mesh_data_copy.vertex_layout  = mesh_data.vertex_layout;

    mesh_data_copy.vertex_list.resize(mesh_data.vertex_list_size);
    std::memcpy(
        mesh_data_copy.vertex_list.data(),
        mesh_data.vertex_list,
        mesh_data.vertex_list_size);

    mesh_data_copy.triangle_list.resize(mesh_data.triangle_list_size);
    std::memcpy(
        mesh_data_copy.triangle_list.data(),
        mesh_data.triangle_list,
        mesh_data.triangle_list_size);
}


static
void
require_same_render_mesh_content (
    const RenderMeshDataCopy &  mesh_data_0,
    const RenderMeshDataCopy &  mesh_data_1)
{
    REQUIRE((mesh_data_0.mesh_id == mesh_data_1.mesh_id));
    REQUIRE((mesh_data_0.bound_box.pmin.x == mesh_data_1.bound_box.pmin.x));
    REQUIRE((mesh_data_0.bound_box.pmin.y == mesh_data_1.bound_box.pmin.y));
    REQUIRE((mesh_data_0.bound_box.pmin.z == mesh_data_1.bound_box.pmin.z));
    REQUIRE((mesh_data_0.bound_box.pmax.x == mesh_data_1.bound_box.pmax.x));
    REQUIRE((mesh_data_0.bound_box.pmax.y == mesh_data_1.bound_box.pmax.y));
    REQUIRE((mesh_data_0.bound_box.pmax.z == mesh_data_1.bound_box.pmax.z));
    REQUIRE((mesh_data_0.vertex_count == mesh_data_1.vertex_count));
    REQUIRE((mesh_data_0.triangle_count == mesh_data_1.triangle_count));
    REQUIRE((mesh_data_0.vertex_layout == mesh_data_1.vertex_layout));
    REQUIRE((mesh_data_0.vertex_list.size() == mesh_data_1.vertex_list.size()));
    REQUIRE((mesh_data_0.triangle_list.size() == mesh_data_1.triangle_list.size()));
    REQUIRE((std::memcmp(
        mesh_data_0.vertex_list.data(),
        mesh_data_1.vertex_list.data(),
        mesh_data_0.vertex_list.size()) == 0));
    REQUIRE((std::memcmp(
        mesh_data_0.triangle_list.data(),
        mesh_data_1.triangle_list.data(),
        mesh_data_0.triangle_list.size()) == 0));
}


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking RMesh File", "[RMeshFile]")
{

    SECTION("Checking RMesh File")
    {
        std::printf("--- Checking RMeshFile...\n");

        char file_name[1024];
        std::snprintf(file_name, sizeof(file_name),
                      "%s/UNIT_TEST/RMeshFile/icosphere.rmesh",
                      NativeDirectory::document_folder());

        /// 通过Depot创建一个Mesh
        const RenderMeshIdT created_mesh_id = RenderMeshDepot::ref().create_unit_icosphere();
        const RenderMesh * const created_mesh_data =
            RenderMeshDepot::ref().mesh_data(created_mesh_id);
        REQUIRE((created_mesh_data));

        /// 制作副本
        RenderMeshDataCopy copied_mesh_data_created;
        copy_render_mesh_data(*created_mesh_data, copied_mesh_data_created);

        /// 创建RMesh文件
        RenderMeshDepot::ref().write_to_mesh_file(file_name, created_mesh_id);
        REQUIRE((NativeFile::does_file_exist(file_name)));
        std::printf("[Output rmesh]: %s\n", file_name);

        /// 清除Depot
        RenderMeshDepot::ref().clear();

        const RenderMeshIdT loaded_mesh_id =
            RenderMeshDepot::ref().create_from_mesh_file(file_name, created_mesh_id);
        REQUIRE((created_mesh_id == loaded_mesh_id));
        const RenderMesh * const loaded_mesh_data =
            RenderMeshDepot::ref().mesh_data(loaded_mesh_id);
        REQUIRE((loaded_mesh_data));

        /// 制作副本
        RenderMeshDataCopy copied_mesh_data_loaded;
        copy_render_mesh_data(*loaded_mesh_data, copied_mesh_data_loaded);
        require_same_render_mesh_content(
            copied_mesh_data_created, copied_mesh_data_loaded);

        /// 将读入的Mesh以Ply形式保存
        std::snprintf(file_name, sizeof(file_name),
                      "%s/UNIT_TEST/RMeshFile/icosphere.ply",
                      NativeDirectory::document_folder());
        RenderMeshDepot::ref().write_to_ply_file(file_name, loaded_mesh_id);
        std::printf("[Output Ply]: %s\n", file_name);

        std::printf("--- Checking RMeshFile: OK!\n");
    }

} /// TEST_CASE("Checking RMesh File", "[RMeshFile]")

#endif /// (RENDER_MESH_FILE_CHECKING == 1)
