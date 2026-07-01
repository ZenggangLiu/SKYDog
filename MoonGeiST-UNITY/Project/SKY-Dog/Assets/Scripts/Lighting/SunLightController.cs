using System;
using UnityEngine;


namespace SKYDog
{

    /// Controls a Unity directional light as a simple sun light.
    ///
    /// The user-provided date and time are interpreted as Greenwich time.
    [ExecuteAlways]
    [DisallowMultipleComponent]
    [RequireComponent(typeof(Light))]
    [DefaultExecutionOrder(-20)]
    public class SunLightController : MonoBehaviour
    {
        // === PUBLIC MEMBERS === //
        [Tooltip("Target directional light. If empty, the Light component on this GameObject is used.")]
        public Light TargetLight;

        [Header("Greenwich Time")]
        [Range(1, 12)]
        public int Month = 6;
        [Range(1, 31)]
        public int Day = 23;
        [Range(0, 23)]
        public int Hour = 12;
        [Range(0, 59)]
        public int Minute = 0;

        [Header("Light")]
        [Tooltip("Maximum directional light intensity used when the sun is high.")]
        public float MaxIntensity = 1.25f;
        [Tooltip("Light color near sunrise and sunset.")]
        public Color HorizonColor = new Color(1.0f, 0.42f, 0.16f, 1.0f);
        [Tooltip("Light color when the sun is high.")]
        public Color DayColor = new Color(1.0f, 0.96f, 0.86f, 1.0f);
        [Tooltip("Sun below this elevation has no direct light.")]
        public float NightElevationDegrees = -6.0f;
        [Tooltip("Sun reaches full direct-light strength at this elevation.")]
        public float FullIntensityElevationDegrees = 15.0f;
        [Tooltip("Sun reaches daytime color at this elevation.")]
        public float DayColorElevationDegrees = 35.0f;

        [Header("Debug")]
        [SerializeField]
        private float m_sun_elevation_degrees;
        [SerializeField]
        private float m_sun_azimuth_degrees;

        // === PRIVATE MEMBERS === //
        private const int FIXED_YEAR = 2026;
        private const int DAYS_PER_MONTH = 30;
        private const float GREENWICH_LATITUDE_DEGREES = 51.4769f;

        private DateTime m_current_greenwich_time;


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


        /// Gets the attached Light component if TargetLight is not specified,
        /// then configures it as the scene sun.
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
            m_current_greenwich_time = make_simulated_date_time(Month, Day, Hour, Minute);
        }


        /// Keeps date and time fields valid.
        private void clamp_date_and_time()
        {
            Month = Mathf.Clamp(Month, 1, 12);
            Day = Mathf.Clamp(Day, 1, DAYS_PER_MONTH);
            Hour = Mathf.Clamp(Hour, 0, 23);
            Minute = Mathf.Clamp(Minute, 0, 59);
        }


        private void update_sun_light()
        {
            if (TargetLight != null)
            {
                SunPosition sun_position = calculate_sun_position(m_current_greenwich_time);

                m_sun_elevation_degrees = sun_position.ElevationDegrees;
                m_sun_azimuth_degrees = sun_position.AzimuthDegrees;

                Vector3 dir_to_sun = sun_position.DirectionToSun;
                Vector3 light_ray_direction = -dir_to_sun;

                if (light_ray_direction.sqrMagnitude > 0.000001f)
                {
                    transform.rotation = Quaternion.LookRotation(light_ray_direction, Vector3.up);
                }

                float intensity_factor =
                    smooth_step(NightElevationDegrees,
                                FullIntensityElevationDegrees,
                                sun_position.ElevationDegrees);
                float color_factor =
                    smooth_step(0.0f,
                                DayColorElevationDegrees,
                                sun_position.ElevationDegrees);

                TargetLight.color = Color.Lerp(HorizonColor, DayColor, color_factor);
                TargetLight.intensity = MaxIntensity * intensity_factor;
            }
        }


        private SunPosition calculate_sun_position(
            DateTime greenwich_time)
        {
            int day_of_year = greenwich_time.DayOfYear;
            float greenwich_hour = greenwich_time.Hour
                                 + greenwich_time.Minute / 60.0f;

            float gamma = 2.0f * Mathf.PI / 365.0f
                        * (day_of_year - 1.0f + (greenwich_hour - 12.0f) / 24.0f);

            float declination =
                0.006918f
              - 0.399912f * Mathf.Cos(gamma)
              + 0.070257f * Mathf.Sin(gamma)
              - 0.006758f * Mathf.Cos(2.0f * gamma)
              + 0.000907f * Mathf.Sin(2.0f * gamma)
              - 0.002697f * Mathf.Cos(3.0f * gamma)
              + 0.001480f * Mathf.Sin(3.0f * gamma);

            float equation_of_time = 229.18f * (
                0.000075f
              + 0.001868f * Mathf.Cos(gamma)
              - 0.032077f * Mathf.Sin(gamma)
              - 0.014615f * Mathf.Cos(2.0f * gamma)
              - 0.040849f * Mathf.Sin(2.0f * gamma));

            float greenwich_minutes = greenwich_time.Hour * 60.0f
                                    + greenwich_time.Minute;
            float true_solar_minutes = greenwich_minutes
                                     + equation_of_time;

            true_solar_minutes = repeat(true_solar_minutes, 1440.0f);

            float hour_angle_degrees = true_solar_minutes / 4.0f - 180.0f;
            float hour_angle = hour_angle_degrees * Mathf.Deg2Rad;
            float latitude = GREENWICH_LATITUDE_DEGREES * Mathf.Deg2Rad;

            float sin_elevation =
                Mathf.Sin(latitude) * Mathf.Sin(declination)
              + Mathf.Cos(latitude) * Mathf.Cos(declination) * Mathf.Cos(hour_angle);
            sin_elevation = Mathf.Clamp(sin_elevation, -1.0f, 1.0f);

            float elevation = Mathf.Asin(sin_elevation);

            float azimuth = Mathf.Atan2(
                Mathf.Sin(hour_angle),
                Mathf.Cos(hour_angle) * Mathf.Sin(latitude)
                    - Mathf.Tan(declination) * Mathf.Cos(latitude));
            azimuth += Mathf.PI;

            Vector3 dir_to_sun = new Vector3(
                Mathf.Cos(elevation) * Mathf.Sin(azimuth),
                Mathf.Sin(elevation),
                Mathf.Cos(elevation) * Mathf.Cos(azimuth));
            dir_to_sun.Normalize();

            SunPosition result;
            result.DirectionToSun = dir_to_sun;
            result.ElevationDegrees = elevation * Mathf.Rad2Deg;
            result.AzimuthDegrees = repeat(azimuth * Mathf.Rad2Deg, 360.0f);
            return result;
        }


        private DateTime make_simulated_date_time(
            int month,
            int day,
            int hour,
            int minute)
        {
            int simulated_day_of_year = (month - 1) * DAYS_PER_MONTH + (day - 1);
            return new DateTime(FIXED_YEAR, 1, 1, hour, minute, 0)
                .AddDays(simulated_day_of_year);
        }


        /// Smooth Hermite interpolation:
        /// t = saturate((value - a) / (b - a))
        /// result = t * t * (3 - 2 * t)
        private float smooth_step(
            float a,
            float b,
            float value)
        {
            float t = Mathf.Clamp01((value - a) / (b - a));
            return t * t * (3.0f - 2.0f * t);
        }


        /// Wraps value into the periodic range [0, length).
        /// Used for clock minutes and azimuth angles.
        private float repeat(
            float value,
            float length)
        {
            return value - Mathf.Floor(value / length) * length;
        }


        private struct SunPosition
        {
            public Vector3 DirectionToSun;
            public float   ElevationDegrees;
            public float   AzimuthDegrees;
        }
    }

} /// namespace SKYDog
