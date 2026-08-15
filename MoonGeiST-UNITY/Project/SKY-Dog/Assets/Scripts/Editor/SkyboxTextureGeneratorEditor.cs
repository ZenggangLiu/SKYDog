using System.IO;
using UnityEditor;
using UnityEngine;


namespace SKYDog.Editor
{

    [CustomEditor(typeof(SkyboxTextureGenerator))]
    public class SkyboxTextureGeneratorEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            DrawDefaultInspector();

            SkyboxTextureGenerator generator = (SkyboxTextureGenerator)target;

            EditorGUILayout.Space();

            if (GUILayout.Button("生成天空盒纹理"))
            {
                generator.GenerateSkyboxTexture();
                EditorUtility.SetDirty(generator);
            }

            if (GUILayout.Button("应用到RenderSettings.skybox"))
            {
                generator.ApplyGeneratedSkybox();
                EditorUtility.SetDirty(generator);
            }

            if (GUILayout.Button("保存Cubemap和Material"))
            {
                save_skybox_assets(generator);
            }
        }


        private void save_skybox_assets(
            SkyboxTextureGenerator generator)
        {
            if (generator.GeneratedCubemap == null)
            {
                generator.GenerateSkyboxTexture();
            }

            string cubemap_path = EditorUtility.SaveFilePanelInProject(
                "保存天空盒Cubemap",
                "GeneratedSkybox",
                "asset",
                "选择保存生成天空盒Cubemap的位置。",
                "Assets");

            if (string.IsNullOrEmpty(cubemap_path))
            {
                return;
            }

            cubemap_path = AssetDatabase.GenerateUniqueAssetPath(cubemap_path);

            Cubemap cubemap_asset = Instantiate(generator.GeneratedCubemap);
            cubemap_asset.name = Path.GetFileNameWithoutExtension(cubemap_path);
            AssetDatabase.CreateAsset(cubemap_asset, cubemap_path);

            string folder = Path.GetDirectoryName(cubemap_path);
            string material_name = cubemap_asset.name + "_Material.mat";
            string material_path = AssetDatabase.GenerateUniqueAssetPath(
                Path.Combine(folder, material_name).Replace("\\", "/"));

            Material material_asset = create_skybox_material(cubemap_asset, generator.SkyboxExposure);

            if (material_asset != null)
            {
                material_asset.name = Path.GetFileNameWithoutExtension(material_path);
                AssetDatabase.CreateAsset(material_asset, material_path);
            }

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();

            Selection.activeObject = material_asset != null ? material_asset : cubemap_asset;
        }


        private Material create_skybox_material(
            Cubemap cubemap,
            float exposure)
        {
            Material result = null;
            Shader skybox_shader = Shader.Find("Skybox/Cubemap");

            if (skybox_shader != null)
            {
                result = new Material(skybox_shader);
                result.SetTexture("_Tex", cubemap);
                result.SetFloat("_Exposure", exposure);
            }
            else
            {
                EditorUtility.DisplayDialog(
                    "无法创建天空盒材质",
                    "没有找到Unity内置Shader: Skybox/Cubemap。",
                    "OK");
            }

            return result;
        }
    }

} /// 命名空间 SKYDog.Editor
