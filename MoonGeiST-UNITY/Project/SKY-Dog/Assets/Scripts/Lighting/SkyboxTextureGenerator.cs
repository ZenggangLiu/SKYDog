using UnityEngine;


namespace SKYDog
{

    /// 根据当前太阳光生成一个程序化Cubemap天空盒。
    [ExecuteAlways]
    [DisallowMultipleComponent]
    [DefaultExecutionOrder(-10)]
    public class SkyboxTextureGenerator : MonoBehaviour
    {
        // === 公开成员 === //
        [Header("Sun")]
        [Tooltip("用于计算天空方向和太阳颜色的方向光。如果为空, 优先使用RenderSettings.sun。")]
        public Light SunLight;

        [Header("Texture")]
        [Tooltip("Cubemap每个面的分辨率。Editor烘焙可以使用256或512, 运行时建议使用128或更低。")]
        [Range(16, 1024)]
        public int CubemapSize = 128;
        [Tooltip("组件启用时自动生成天空盒。")]
        public bool GenerateOnEnable = true;
        [Tooltip("每帧重新生成天空盒。只有太阳参数连续变化且分辨率较低时才建议开启。")]
        public bool RegenerateEveryFrame = false;
        [Tooltip("生成后自动设置到RenderSettings.skybox。")]
        public bool ApplyToRenderSettings = true;
        [Tooltip("天空盒改变后刷新环境光。运行时频繁刷新会比较昂贵。")]
        public bool UpdateGlobalIllumination = false;

        [Header("Hosek/Wilkie")]
        [Tooltip("大气浑浊度。1是非常清澈, 10是非常浑浊。")]
        [Range(1.0f, 10.0f)]
        public float AtmosphericTurbidity = 3.0f;
        [Tooltip("地面反照率。会影响天空整体亮度和地平线色彩。")]
        [Range(0.0f, 1.0f)]
        public float GroundAlbedo = 0.1f;
        [Tooltip("Hosek/Wilkie RGB模型输出的HDR缩放。")]
        public float SkyRadianceScale = 0.04f;
        [Tooltip("天空整体色彩倍率。")]
        public Color SkyTint = Color.white;

        [Header("Night Sky")]
        [Tooltip("夜晚天顶颜色。")]
        public Color NightZenithColor = new Color(0.005f, 0.012f, 0.035f, 1.0f);
        [Tooltip("夜晚地平线颜色。")]
        public Color NightHorizonColor = new Color(0.025f, 0.025f, 0.055f, 1.0f);
        [Tooltip("夜晚地平线到天顶的过渡曲线。")]
        [Range(0.25f, 8.0f)]
        public float NightGradientPower = 1.6f;

        [Header("Ground")]
        [Tooltip("地面方向接近地平线时的颜色。")]
        public Color GroundHorizonColor = new Color(0.28f, 0.30f, 0.34f, 1.0f);
        [Tooltip("地面正下方颜色。")]
        public Color GroundBottomColor = new Color(0.055f, 0.055f, 0.06f, 1.0f);
        [Tooltip("地平线到地面下方的过渡曲线。")]
        [Range(0.25f, 8.0f)]
        public float GroundGradientPower = 1.2f;

        [Header("Sun Disc")]
        [Tooltip("太阳圆盘半径。真实太阳约为0.265度, 但天空盒中通常会放大以获得更清晰的视觉效果。")]
        [Range(0.05f, 10.0f)]
        public float SunDiscRadiusDegrees = 1.0f;
        [Tooltip("太阳圆盘边缘软化范围。")]
        [Range(0.01f, 10.0f)]
        public float SunDiscSoftnessDegrees = 0.35f;
        [Tooltip("太阳圆盘亮度倍率。")]
        [Range(0.0f, 32.0f)]
        public float SunDiscIntensity = 6.0f;
        [Tooltip("太阳光晕半径。")]
        [Range(1.0f, 90.0f)]
        public float SunHaloRadiusDegrees = 24.0f;
        [Tooltip("太阳光晕亮度倍率。")]
        [Range(0.0f, 8.0f)]
        public float SunHaloIntensity = 0.65f;
        [Tooltip("太阳光晕衰减曲线。数值越大, 光晕越集中在太阳附近。")]
        [Range(0.25f, 16.0f)]
        public float SunHaloPower = 5.0f;

        [Header("Day/Night")]
        [Tooltip("太阳天顶角大于等于此值时, 天空进入夜晚。")]
        public float NightThetaDegrees = 96.0f;
        [Tooltip("太阳天顶角小于等于此值时, 天空完全进入白天。")]
        public float FullDayThetaDegrees = 75.0f;

        [Header("Material")]
        [Tooltip("天空盒材质曝光。")]
        public float SkyboxExposure = 1.0f;

        // === 私有成员 === //
        private Cubemap m_generated_cubemap;
        private Material m_generated_skybox_material;

        public Cubemap GeneratedCubemap
        {
            get { return m_generated_cubemap; }
        }


        public Material GeneratedSkyboxMaterial
        {
            get { return m_generated_skybox_material; }
        }


        private void OnEnable()
        {
            if (GenerateOnEnable)
            {
                generate_and_optionally_apply();
            }
        }


        private void OnValidate()
        {
            CubemapSize = Mathf.Clamp(CubemapSize, 16, 1024);
        }


        private void Update()
        {
            if (RegenerateEveryFrame)
            {
                generate_and_optionally_apply();
            }
        }


        private void OnDestroy()
        {
            release_generated_objects();
        }


        public void GenerateSkyboxTexture()
        {
            Light sun_light = get_sun_light();
            Vector3 direction_to_earth = get_direction_to_earth(sun_light);
            Vector3 direction_to_sun = -direction_to_earth;
            Color sun_color = get_sun_color(sun_light);
            float sun_cos_theta = Mathf.Clamp(Vector3.Dot(Vector3.up, direction_to_sun), -1.0f, 1.0f);
            float solar_elevation = Mathf.Asin(sun_cos_theta);
            HosekWilkieSkyModel.State sky_state = HosekWilkieSkyModel.CreateState(
                AtmosphericTurbidity,
                GroundAlbedo,
                Mathf.Max(0.0f, solar_elevation));
            float daylight_scale = calc_daylight_scale(sun_cos_theta);

            ensure_cubemap();

            for (int face_idx = 0; face_idx < 6; ++face_idx)
            {
                CubemapFace face = (CubemapFace)face_idx;
                Color[] pixels = new Color[CubemapSize * CubemapSize];

                for (int y = 0; y < CubemapSize; ++y)
                {
                    for (int x = 0; x < CubemapSize; ++x)
                    {
                        Vector3 sample_dir = calc_cubemap_direction(face, x, y);
                        pixels[y * CubemapSize + x] = sample_sky_color(
                            sample_dir,
                            direction_to_sun,
                            sky_state,
                            sun_color,
                            daylight_scale);
                    }
                }

                m_generated_cubemap.SetPixels(pixels, face);
            }

            m_generated_cubemap.Apply(false, false);
        }


        public void ApplyGeneratedSkybox()
        {
            if (m_generated_cubemap != null)
            {
                ensure_skybox_material();

                if (m_generated_skybox_material != null)
                {
                    m_generated_skybox_material.SetTexture("_Tex", m_generated_cubemap);
                    m_generated_skybox_material.SetFloat("_Exposure", SkyboxExposure);
                    RenderSettings.skybox = m_generated_skybox_material;

                    if (UpdateGlobalIllumination)
                    {
                        DynamicGI.UpdateEnvironment();
                    }
                }
            }
        }


        private void generate_and_optionally_apply()
        {
            GenerateSkyboxTexture();

            if (ApplyToRenderSettings)
            {
                ApplyGeneratedSkybox();
            }
        }


        private Light get_sun_light()
        {
            Light result = SunLight;

            if (result == null)
            {
                result = RenderSettings.sun;
            }

            if (result == null)
            {
                result = GetComponent<Light>();
            }

            return result;
        }


        private Vector3 get_direction_to_earth(
            Light sun_light)
        {
            Vector3 result = Vector3.down;

            if (sun_light != null)
            {
                result = sun_light.transform.forward;
            }

            if (result.sqrMagnitude > 0.000001f)
            {
                result.Normalize();
            }

            return result;
        }


        private Color get_sun_color(
            Light sun_light)
        {
            Color result = Color.white;

            if (sun_light != null)
            {
                result = sun_light.color * Mathf.Max(0.0f, sun_light.intensity);
            }

            return result;
        }


        private float calc_daylight_scale(
            float sun_cos_theta)
        {
            float night_cos_theta = Mathf.Cos(NightThetaDegrees * Mathf.Deg2Rad);
            float full_day_cos_theta = Mathf.Cos(FullDayThetaDegrees * Mathf.Deg2Rad);
            return smooth_step(night_cos_theta, full_day_cos_theta, sun_cos_theta);
        }


        private void ensure_cubemap()
        {
            if (m_generated_cubemap != null && m_generated_cubemap.width == CubemapSize)
            {
                return;
            }

            destroy_object(m_generated_cubemap);
            m_generated_cubemap = new Cubemap(CubemapSize, TextureFormat.RGBAHalf, false);
            m_generated_cubemap.name = "Generated Procedural Skybox";
            m_generated_cubemap.wrapMode = TextureWrapMode.Clamp;
            m_generated_cubemap.filterMode = FilterMode.Trilinear;
            m_generated_cubemap.hideFlags = HideFlags.DontSave;
        }


        private void ensure_skybox_material()
        {
            if (m_generated_skybox_material == null)
            {
                Shader skybox_shader = Shader.Find("Skybox/Cubemap");

                if (skybox_shader != null)
                {
                    m_generated_skybox_material = new Material(skybox_shader);
                    m_generated_skybox_material.name = "Generated Procedural Skybox Material";
                    m_generated_skybox_material.hideFlags = HideFlags.DontSave;
                }
            }
        }


        private Color sample_sky_color(
            Vector3 sample_dir,
            Vector3 direction_to_sun,
            HosekWilkieSkyModel.State sky_state,
            Color sun_color,
            float daylight_scale)
        {
            float up = sample_dir.y;
            Color base_color = sample_base_sky_color(sample_dir, direction_to_sun, sky_state, daylight_scale);

            float sun_dot = Mathf.Clamp(Vector3.Dot(sample_dir, direction_to_sun), -1.0f, 1.0f);
            Color sun_disc_color = calc_sun_disc_color(sun_dot, sun_color, daylight_scale);
            Color sun_halo_color = calc_sun_halo_color(sun_dot, sun_color, daylight_scale);

            return base_color + sun_halo_color + sun_disc_color;
        }


        private Color sample_base_sky_color(
            Vector3 sample_dir,
            Vector3 direction_to_sun,
            HosekWilkieSkyModel.State sky_state,
            float daylight_scale)
        {
            float up = sample_dir.y;
            Color result;

            if (up >= 0.0f)
            {
                float theta = Mathf.Acos(Mathf.Clamp(up, 0.0f, 1.0f));
                float gamma = Mathf.Acos(Mathf.Clamp(Vector3.Dot(sample_dir, direction_to_sun), -1.0f, 1.0f));
                Color day_color = HosekWilkieSkyModel.Evaluate(
                    sky_state,
                    theta,
                    gamma,
                    SkyRadianceScale);
                day_color.r *= SkyTint.r;
                day_color.g *= SkyTint.g;
                day_color.b *= SkyTint.b;

                float gradient = Mathf.Pow(Mathf.Clamp01(up), NightGradientPower);
                Color night_color = Color.Lerp(NightHorizonColor, NightZenithColor, gradient);
                result = Color.Lerp(night_color, day_color, daylight_scale);
            }
            else
            {
                float gradient = Mathf.Pow(Mathf.Clamp01(-up), GroundGradientPower);
                Color day_color = Color.Lerp(GroundHorizonColor, GroundBottomColor, gradient);
                Color night_color = Color.Lerp(NightHorizonColor, NightZenithColor, gradient);
                result = Color.Lerp(night_color, day_color, daylight_scale);
            }

            result.a = 1.0f;
            return result;
        }


        private Color calc_sun_disc_color(
            float sun_dot,
            Color sun_color,
            float daylight_scale)
        {
            float inner_radius = SunDiscRadiusDegrees;
            float outer_radius = SunDiscRadiusDegrees + SunDiscSoftnessDegrees;
            float inner_cos = Mathf.Cos(inner_radius * Mathf.Deg2Rad);
            float outer_cos = Mathf.Cos(outer_radius * Mathf.Deg2Rad);
            float disc_factor = smooth_step(outer_cos, inner_cos, sun_dot);

            Color result = sun_color * (SunDiscIntensity * daylight_scale * disc_factor);
            result.a = disc_factor;
            return result;
        }


        private Color calc_sun_halo_color(
            float sun_dot,
            Color sun_color,
            float daylight_scale)
        {
            float halo_cos = Mathf.Cos(SunHaloRadiusDegrees * Mathf.Deg2Rad);
            float halo_factor = smooth_step(halo_cos, 1.0f, sun_dot);
            halo_factor = Mathf.Pow(halo_factor, SunHaloPower);

            Color result = sun_color * (SunHaloIntensity * daylight_scale * halo_factor);
            result.a = halo_factor;
            return result;
        }


        private Vector3 calc_cubemap_direction(
            CubemapFace face,
            int x,
            int y)
        {
            float u = ((x + 0.5f) / CubemapSize) * 2.0f - 1.0f;
            float v = ((y + 0.5f) / CubemapSize) * 2.0f - 1.0f;
            Vector3 result = Vector3.forward;

            if (face == CubemapFace.PositiveX)
            {
                result = new Vector3(1.0f, -v, -u);
            }
            else if (face == CubemapFace.NegativeX)
            {
                result = new Vector3(-1.0f, -v, u);
            }
            else if (face == CubemapFace.PositiveY)
            {
                result = new Vector3(u, 1.0f, v);
            }
            else if (face == CubemapFace.NegativeY)
            {
                result = new Vector3(u, -1.0f, -v);
            }
            else if (face == CubemapFace.PositiveZ)
            {
                result = new Vector3(u, -v, 1.0f);
            }
            else if (face == CubemapFace.NegativeZ)
            {
                result = new Vector3(-u, -v, -1.0f);
            }

            result.Normalize();
            return result;
        }


        private float smooth_step(
            float a,
            float b,
            float value)
        {
            float t = Mathf.Clamp01((value - a) / (b - a));
            return t * t * (3.0f - 2.0f * t);
        }


        private void release_generated_objects()
        {
            if (RenderSettings.skybox == m_generated_skybox_material)
            {
                RenderSettings.skybox = null;
            }

            destroy_object(m_generated_cubemap);
            destroy_object(m_generated_skybox_material);
            m_generated_cubemap = null;
            m_generated_skybox_material = null;
        }


        private void destroy_object(
            Object obj)
        {
            if (obj != null)
            {
                if (Application.isPlaying)
                {
                    Destroy(obj);
                }
                else
                {
                    DestroyImmediate(obj);
                }
            }
        }
    }

} /// 命名空间 SKYDog
