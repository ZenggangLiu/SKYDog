using System;
using UnityEngine;


namespace SKYDog
{

    /// 将Unity方向光作为简单太阳光控制。
    ///
    /// 用户提供的日期和时间按天津本地时间解释。
    [ExecuteAlways]
    [DisallowMultipleComponent]
    [RequireComponent(typeof(Light))]
    [DefaultExecutionOrder(-20)]
    public class SunLightController : MonoBehaviour
    {
        // === 公开成员 === //
        [Tooltip("目标方向光。如果为空, 使用当前游戏对象上的Light组件。")]
        public Light TargetLight;

        [Header("天津本地时间")]
        [Range(1, 12)]
        public int Month = 10;
        [Range(1, 31)]
        public int Day = 17;
        [Range(0, 23)]
        public int Hour = 10;
        [Range(0, 59)]
        public int Minute = 30;

        [Header("光照")]
        [Tooltip("太阳较高时使用的最大方向光强度。")]
        public float MaxIntensity = 1.25f;
        [Tooltip("日出和日落附近的光照颜色。")]
        public Color HorizonColor = new Color(1.0f, 0.42f, 0.16f, 1.0f);
        [Tooltip("太阳较高时的光照颜色。")]
        public Color DayColor = new Color(1.0f, 0.96f, 0.86f, 1.0f);
        [Tooltip("太阳天顶角大于等于此值时, 直射光强度为0。天顶角从世界上方向(+Y)开始量。")]
        public float NightThetaDegrees = 96.0f;
        [Tooltip("太阳天顶角小于等于此值时, 直射光达到最大强度。天顶角越小表示太阳越高。")]
        public float FullIntensityThetaDegrees = 75.0f;
        [Tooltip("太阳天顶角小于等于此值时, 使用白天光照颜色。")]
        public float DayColorThetaDegrees = 55.0f;

        [Header("调试")]
        [SerializeField]
        private float m_sun_cos_theta;
        [SerializeField]
        private Vector3 m_sun_direction_to_sun;

        // === 私有成员 === //
        private const int FIXED_YEAR = 1973;
        private const float LOCATION_LATITUDE_DEGREES = 39.1333f;
        private const float LOCATION_LONGITUDE_DEGREES = 117.1833f;
        private const float LOCATION_TIME_ZONE_HOURS = 8.0f;

        private static readonly Vector3 LOCATION_UP = make_location_up();
        private static readonly Vector3 LOCATION_EAST = make_location_east();
        private static readonly Vector3 LOCATION_NORTH = make_location_north();

        private DateTime m_current_tianjin_time;


        private void Awake()
        {
            get_light();
            reset_current_time();
            update_sun_light();
        }


        private void OnValidate()
        {
            get_light();
            clamp_date_and_time();
            reset_current_time();
            update_sun_light();
        }


        private void Update()
        {
            reset_current_time();
            update_sun_light();
        }


        /// 如果TargetLight没有指定, 获取挂接的Light组件,
        /// 然后将其配置为场景太阳光。
        private void get_light()
        {
            if (TargetLight == null)
            {
                TargetLight = GetComponent<Light>();
            }

            if (TargetLight != null)
            {
                TargetLight.type = LightType.Directional;
                RenderSettings.sun = TargetLight;
            }
        }


        private void reset_current_time()
        {
            clamp_date_and_time();
            m_current_tianjin_time = make_date_time(Month, Day, Hour, Minute);
        }


        /// 保证日期和时间字段有效。
        private void clamp_date_and_time()
        {
            Month = Mathf.Clamp(Month, 1, 12);
            Day = Mathf.Clamp(Day, 1, DateTime.DaysInMonth(FIXED_YEAR, Month));
            Hour = Mathf.Clamp(Hour, 0, 23);
            Minute = Mathf.Clamp(Minute, 0, 59);
        }


        private void update_sun_light()
        {
            if (TargetLight != null)
            {
                SunPosition sun_position = calculate_sun_position(m_current_tianjin_time);

                m_sun_cos_theta = sun_position.CosTheta;
                m_sun_direction_to_sun = sun_position.DirectionToSun;

                Vector3 dir_to_sun = sun_position.DirectionToSun;
                Vector3 light_ray_direction = -dir_to_sun;

                if (light_ray_direction.sqrMagnitude > 0.000001f)
                {
                    transform.rotation = Quaternion.LookRotation(light_ray_direction, Vector3.up);
                }

                /// 天顶角(θ)越小, cos(θ)越大。
                /// 因此直接用cos(θ)做光强插值:
                /// - cos(θ) <= cos(NightThetaDegrees)时, Intensity为0
                /// - cos(θ) >= cos(FullIntensityThetaDegrees)时, Intensity为1
                float night_cos_theta = Mathf.Cos(NightThetaDegrees * Mathf.Deg2Rad);
                float full_intensity_cos_theta = Mathf.Cos(FullIntensityThetaDegrees * Mathf.Deg2Rad);
                float intensity_factor =
                    smooth_step(night_cos_theta,
                                full_intensity_cos_theta,
                                sun_position.CosTheta);

                /// 地平线处cos(θ)为0, 太阳升高后逐渐过渡到白天颜色。
                float day_color_cos_theta = Mathf.Cos(DayColorThetaDegrees * Mathf.Deg2Rad);
                float color_factor =
                    smooth_step(0.0f,
                                day_color_cos_theta,
                                sun_position.CosTheta);

                TargetLight.color = Color.Lerp(HorizonColor, DayColor, color_factor);
                TargetLight.intensity = MaxIntensity * intensity_factor;
            }
        }


        private SunPosition calculate_sun_position(
            DateTime location_time_info)
        {
            /// SUN的方位:
            ///   上方
            ///   Y
            ///   ^   Z
            ///   |  / 北方
            ///   | /
            ///   |/      X
            ///   O-------> 东方
            ///
            /// 指定日期在一年中是第几天(从0开始)
            int day_of_year = location_time_info.DayOfYear - 1; /// 289: 1973:10月17日
            float tianjin_hour = location_time_info.Hour
                               + location_time_info.Minute / 60.0f;

            ///      2*π                     hour - 12
            /// γ = ────── * (day_of_year + ────────────
            ///      365                         24
            /// 说明:
            /// day_of_year: 从0开始统计的指定日期在一年中是第几天
            float gamma = 2.0f * Mathf.PI / 365.0f
                        * (day_of_year + (tianjin_hour - 12.0f) / 24.0f);

            /// 从γ可以计算时间方程(分钟):
            /// 时间方程 =
            ///     229.18 *
            ///     (0.000075 +
            ///      0.001868*cos( γ) – 0.032077*sin( γ) –
            ///      0.014615*cos(2γ) – 0.040849*sin(2γ))
            ///
            float equation_of_time = 229.18f * (
                0.000075f
              + 0.001868f * Mathf.Cos(gamma)
              - 0.032077f * Mathf.Sin(gamma)
              - 0.014615f * Mathf.Cos(2.0f * gamma)
              - 0.040849f * Mathf.Sin(2.0f * gamma));

            /// 太阳Declination(弧度):
            /// Declination =
            ///     0.006918 –
            ///     0.399912*cos(  γ) + 0.070257*sin(  γ) –
            ///     0.006758*cos(2*γ) + 0.000907*sin(2*γ) –
            ///     0.002697*cos(3*γ) + 0.001480*sin(3*γ)
            ///
            float declination =
                0.006918f
              - 0.399912f * Mathf.Cos(gamma)
              + 0.070257f * Mathf.Sin(gamma)
              - 0.006758f * Mathf.Cos(2.0f * gamma)
              + 0.000907f * Mathf.Sin(2.0f * gamma)
              - 0.002697f * Mathf.Cos(3.0f * gamma)
              + 0.001480f * Mathf.Sin(3.0f * gamma);

            /// 先把天津本地时间转换为格林威治时间。
            /// 然后用格林威治真太阳时得到日下点所在经度。
            float location_mins = location_time_info.Hour * 60.0f + location_time_info.Minute;
            float utc_mins = location_mins - LOCATION_TIME_ZONE_HOURS * 60.0f;
            float greenwich_true_solar_mins = repeat(utc_mins + equation_of_time, 1440.0f);

            /// 格林威治时角(度) := 格林威治真太阳时间 / 4 - 180
            /// 日下点经度与格林威治时角相反。
            float greenwich_hour_angle_degs = greenwich_true_solar_mins / 4.0f - 180.0f;
            float subsolar_longitude_degs = -greenwich_hour_angle_degs;

            /// 在地心坐标中构造太阳方向。
            /// X: 经度0°, 纬度0°方向
            /// Y: 经度90°E, 纬度0°方向
            /// Z: 北极方向
            ///
            /// 日下点的纬度就是太阳Declination, 经度由真太阳时得到。
            float subsolar_longitude_rads = subsolar_longitude_degs * Mathf.Deg2Rad;
            float sin_declination = Mathf.Sin(declination);
            float cos_declination = Mathf.Cos(declination);
            Vector3 earth_dir_to_sun = new Vector3(
                cos_declination * Mathf.Cos(subsolar_longitude_rads),
                cos_declination * Mathf.Sin(subsolar_longitude_rads),
                sin_declination);

            /// 将地心太阳方向投影到天津本地坐标:
            /// X: 东方, Y: 上方, Z: 北方。
            /// 这样得到的Y分量就是cos(天顶角), 不需要再用反三角函数求θ。
            Vector3 dir_to_sun = new Vector3(
                Vector3.Dot(earth_dir_to_sun, LOCATION_EAST),
                Vector3.Dot(earth_dir_to_sun, LOCATION_UP),
                Vector3.Dot(earth_dir_to_sun, LOCATION_NORTH));
            dir_to_sun.Normalize();

            SunPosition result;
            result.DirectionToSun = dir_to_sun;
            result.CosTheta = Mathf.Clamp(dir_to_sun.y, -1.0f, 1.0f);
            return result;
        }


        private static Vector3 make_location_up()
        {
            float latitude_rads = LOCATION_LATITUDE_DEGREES * Mathf.Deg2Rad;
            float longitude_rads = LOCATION_LONGITUDE_DEGREES * Mathf.Deg2Rad;

            return new Vector3(
                Mathf.Cos(latitude_rads) * Mathf.Cos(longitude_rads),
                Mathf.Cos(latitude_rads) * Mathf.Sin(longitude_rads),
                Mathf.Sin(latitude_rads));
        }


        private static Vector3 make_location_east()
        {
            float longitude_rads = LOCATION_LONGITUDE_DEGREES * Mathf.Deg2Rad;

            return new Vector3(
                -Mathf.Sin(longitude_rads),
                 Mathf.Cos(longitude_rads),
                 0.0f);
        }


        private static Vector3 make_location_north()
        {
            float latitude_rads = LOCATION_LATITUDE_DEGREES * Mathf.Deg2Rad;
            float longitude_rads = LOCATION_LONGITUDE_DEGREES * Mathf.Deg2Rad;

            return new Vector3(
                -Mathf.Sin(latitude_rads) * Mathf.Cos(longitude_rads),
                -Mathf.Sin(latitude_rads) * Mathf.Sin(longitude_rads),
                 Mathf.Cos(latitude_rads));
        }


        private DateTime make_date_time(
            int month,
            int day,
            int hour,
            int minute)
        {
            return new DateTime(FIXED_YEAR, month, day, hour, minute, 0);
        }


        /// 使用Hermite插值来计算value在a, b之间的系数[0, 1]
        ///
        /// t = Clamp01((value - a) / (b - a)) 在[0, 1]
        /// 返回值 = t * t * (3 - 2 * t)
        private float smooth_step(
            float a,
            float b,
            float value)
        {
            float t = Mathf.Clamp01((value - a) / (b - a));
            return t * t * (3.0f - 2.0f * t);
        }


        /// 将value折回周期范围[0, length)。
        /// 用于一天中的分钟数。
        private float repeat(
            float value,
            float length)
        {
            return value - Mathf.Floor(value / length) * length;
        }


        private struct SunPosition
        {
            public Vector3 DirectionToSun;
            public float   CosTheta;
        }
    }

} /// 命名空间 SKYDog
