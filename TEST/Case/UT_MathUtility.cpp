#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (MATH_UTILITY_CHECKING == 1)
/// System headers
#include <cmath>    /// std::acos
#include <cstdio>   /// std::printf
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Math/MathUtilities.hpp"
#include "Time/ClockTime.hpp"


#define CHECK_ARC_COSINE    1
#define CHECK_SINE_COSINE   1


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking MathUtility", "[MathUtility]")
{

#if (CHECK_ARC_COSINE == 1)
    SECTION("Checking Fast Arc-Cosine")
    {
        std::printf("--- Checking Fast Arc-Cosine...\n");

        const uint32_t check_count = 1000000000;
        const double delta = 2.0 / check_count;

        /// 检查性能
        {
            float accu_delta = 0;
            const float start_time_own = ClockTime::mono_time_ms();

            float angle_own;
            for (uint32_t i = 0; i < check_count; ++i)
            {
                angle_own = MathUtility::fast_acos((float)(-1.f + i * delta));
                accu_delta += angle_own;
            }
            angle_own = MathUtility::fast_acos(-1.f);
            accu_delta += angle_own;
            angle_own = MathUtility::fast_acos(1.f);
            accu_delta += angle_own;

            const float end_time_own = ClockTime::mono_time_ms();
            const float used_time_own = end_time_own - start_time_own;
            std::printf("[Time(Own)]: %.4fms, %.8fs, [Delta(DUMMY)]: %f\n",
                        used_time_own, used_time_own/1000, accu_delta);

            accu_delta = 0;
            const float start_time_std = ClockTime::mono_time_ms();

            float angle_std;
            for (uint32_t i = 0; i < check_count; ++i)
            {
                angle_std = (float)std::acos((float)(-1.f + i * delta));
                accu_delta += angle_std;
            }
            angle_std = (float)std::acos(-1.f);
            accu_delta += angle_std;
            angle_std = (float)std::acos(1.f);
            accu_delta += angle_std;

            const float end_time_std = ClockTime::mono_time_ms();
            const float used_time_std = end_time_std - start_time_std;
            std::printf("[Time(Std)]: %.4fms, %.8fs, [Delta(DUMMY)]: %f\n",
                        used_time_std, used_time_std/1000, accu_delta);
        }

        /// 检查精度
        {
            float accu_delta = 0;
            float angle_std;
            float angle_own;
            for (uint32_t i = 0; i < check_count; ++i)
            {
                angle_std = (float) std::acos((float)(-1.f + i * delta));
                angle_own = MathUtility::fast_acos((float)(-1.f + i * delta));
                accu_delta += std::fabsf(angle_std - angle_own);
            }
            angle_std = (float)std::acos(-1.f);
            angle_own = MathUtility::fast_acos(-1.f);
            accu_delta += std::fabsf(angle_std - angle_own);
            angle_std = (float)std::acos(1.f);
            angle_own = MathUtility::fast_acos(1.f);
            accu_delta += std::fabsf(angle_std - angle_own);

            const float delta_rads = accu_delta / (check_count + 2);
            std::printf("[Delta(Rads)]: %f, [Delta(Degs)]: %f, [#Comp]: %u\n",
                        delta_rads,
                        RADIAN_TO_DEGREE(delta_rads),
                        (check_count + 2));
            REQUIRE((delta_rads <= 0.001f));
        }

        std::printf("--- Checking Fast Arc-Cosine: OK!\n");
    } // SECTION("Checking Fast Arc-Cosine")
#endif /// (CHECK_ARC_COSINE == 1)


#if (CHECK_SINE_COSINE == 1)
    SECTION("Checking Combined Sine Cosine")
    {
        /// 目前一亿次
        const uint32_t check_count = 100000000;
        /// 306度: 2π
        const double delta_rads = TWO_PI / check_count;


        std::printf("--- Checking Cosine...\n");
        {
            /// 检查性能
            {
                float accu_delta = 0;
                const float start_time_own = ClockTime::mono_time_ms();

                float cosine_own;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    cosine_own = MathUtility::fast_cosine((float)(i * delta_rads));
                    accu_delta += cosine_own;
                }
                cosine_own = MathUtility::fast_cosine(0.0f);
                accu_delta += cosine_own;
                cosine_own = MathUtility::fast_cosine(TWO_PI);
                accu_delta += cosine_own;

                const float end_time_own  = ClockTime::mono_time_ms();
                const float used_time_own = end_time_own - start_time_own;
                std::printf("[Time(Own)]: %.4fms, %.8fs, [Delta(DUMMY)]: %f\n",
                            used_time_own, used_time_own / 1000.0f, accu_delta);

                accu_delta = 0;
                const float start_time_std = ClockTime::mono_time_ms();

                float cosine_std;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    cosine_std = std::cos((float)(i * delta_rads));
                    accu_delta += cosine_std;
                }
                cosine_std = std::cos(0.0f);
                accu_delta += cosine_std;
                cosine_std = std::cos(TWO_PI);
                accu_delta += cosine_std;

                const float end_time_std  = ClockTime::mono_time_ms();
                const float used_time_std = end_time_std - start_time_std;
                std::printf("[Time(Std)]: %.4fms, %.8fs, [Delta(DUMMY)]: %f\n",
                            used_time_std, used_time_std / 1000.0f, accu_delta);
            }

            /// 检查精度
            {
                float accu_delta = 0;
                float cosine_own;
                float cosine_std;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    cosine_own = MathUtility::fast_cosine((float)(i * delta_rads));
                    cosine_std = std::cos((float)(i * delta_rads));
                    accu_delta += std::fabsf(cosine_std - cosine_own);
                }
                cosine_own = MathUtility::fast_cosine(0.0f);
                cosine_std = std::cos(0.0f);
                accu_delta += std::fabsf(cosine_std - cosine_own);
                cosine_own = MathUtility::fast_cosine(TWO_PI);
                cosine_std = std::cos(TWO_PI);
                accu_delta += std::fabsf(cosine_std - cosine_own);

                const float delta_cos = accu_delta / (check_count + 2);
                std::printf("[Delta(Cosine)]: %f, [#Comp]: %u\n",
                            delta_cos, (check_count + 2));
                REQUIRE((delta_cos < 1e-5));
            }
        }
        std::printf("--- Checking Cosine: OK!\n");

        std::printf("--- Checking Sine...\n");
        {
            /// 检查性能
            {
                float accu_delta = 0;
                const float start_time_own = ClockTime::mono_time_ms();

                float sine_own;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    sine_own = MathUtility::fast_sine((float)(i * delta_rads));
                    accu_delta += sine_own;
                }
                sine_own = MathUtility::fast_sine(0.0f);
                accu_delta += sine_own;
                sine_own = MathUtility::fast_sine(TWO_PI);
                accu_delta += sine_own;

                const float end_time_own  = ClockTime::mono_time_ms();
                const float used_time_own = end_time_own - start_time_own;
                std::printf("[Time(Own)]: %.4fms, %.8fs, [Delta(DUMMY)]: %f\n",
                            used_time_own, used_time_own / 1000, accu_delta);

                accu_delta = 0;
                const float start_time_std = ClockTime::mono_time_ms();

                float sine_std;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    sine_std = std::sin((float)(i * delta_rads));
                    accu_delta += sine_std;
                }
                sine_std = std::sin(0.0f);
                accu_delta += sine_std;
                sine_std = std::sin(TWO_PI);
                accu_delta += sine_std;

                const float end_time_std  = ClockTime::mono_time_ms();
                const float used_time_std = end_time_std - start_time_std;
                std::printf("[Time(Std)]: %.4fms, %.8fs, [Delta(DUMMY)]: %f\n",
                            used_time_std, used_time_std / 1000, accu_delta);
            }

            /// 检查精度
            {
                float accu_delta = 0;
                float sine_own;
                float sine_std;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    sine_own = MathUtility::fast_sine((float)(i * delta_rads));
                    sine_std = std::sin((float)(i * delta_rads));
                    accu_delta += std::fabsf(sine_std - sine_own);
                }
                sine_own = MathUtility::fast_sine(0.0f);
                sine_std = std::sin(0.0f);
                accu_delta += std::fabsf(sine_std - sine_own);
                sine_own = MathUtility::fast_sine(TWO_PI);
                sine_std = std::sin(TWO_PI);
                accu_delta += std::fabsf(sine_std - sine_own);

                const float delta_sin = accu_delta / (check_count + 2);
                std::printf("[Delta(Sine)]: %f, [#Comp]: %u\n",
                            delta_sin, (check_count + 2));
                REQUIRE((delta_sin < 1e-5));
            }
        }
        std::printf("--- Checking Sine: OK!\n");

        std::printf("--- Checking Combined Sine Cosine...\n");
        {
            /// 检查性能
            {
                float accu_delta = 0;
                const float start_time_own = ClockTime::mono_time_ms();

                float sine_own, cosine_own;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    MathUtility::fast_sincos((float)(i * delta_rads), sine_own, cosine_own);
                    accu_delta += sine_own + cosine_own;
                }
                MathUtility::fast_sincos(0.0f, sine_own, cosine_own);
                accu_delta += sine_own + cosine_own;
                MathUtility::fast_sincos(TWO_PI, sine_own, cosine_own);
                accu_delta += sine_own + cosine_own;

                const float end_time_own  = ClockTime::mono_time_ms();
                const float used_time_own = end_time_own - start_time_own;
                std::printf("[Time(Own)]: %.4fms, %.8fs, [Delta(DUMMY)]: %f\n",
                            used_time_own, used_time_own / 1000, accu_delta);

                accu_delta = 0;
                const float start_time_std = ClockTime::mono_time_ms();

                float sine_std, cosine_std;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    sine_std   = std::sin((float)(i * delta_rads));
                    cosine_std = std::cos((float)(i * delta_rads));
                    accu_delta += sine_std + cosine_std;
                }
                sine_std   = std::sin(0.0f);
                cosine_std = std::cos(0.0f);
                accu_delta += sine_std + cosine_std;
                sine_std   = std::sin(TWO_PI);
                cosine_std = std::cos(TWO_PI);
                accu_delta += sine_std + cosine_std;

                const float end_time_std  = ClockTime::mono_time_ms();
                const float used_time_std = end_time_std - start_time_std;
                std::printf("[Time(Std)]: %.4fms, %.8fs, [Delta(DUMMY)]: %f\n",
                            used_time_std, used_time_std / 1000, accu_delta);
            }

            /// 检查精度
            {
                float accu_sine_delta = 0;
                float accu_cosine_delta = 0;
                float sine_own, cosine_own, sine_std, cosine_std;
                for (uint32_t i = 0; i < check_count; ++i)
                {
                    MathUtility::fast_sincos((float)(i * delta_rads), sine_own, cosine_own);
                    sine_std   = std::sin((float)(i * delta_rads));
                    cosine_std = std::cos((float)(i * delta_rads));
                    accu_sine_delta   += std::fabsf(sine_std - sine_own);
                    accu_cosine_delta += std::fabsf(cosine_std - cosine_own);
                }
                MathUtility::fast_sincos(0.0f, sine_own, cosine_own);
                sine_std   = std::sin(0.0f);
                cosine_std = std::cos(0.0f);
                accu_sine_delta   += std::fabsf(sine_std   - sine_own);
                accu_cosine_delta += std::fabsf(cosine_std - cosine_own);
                MathUtility::fast_sincos(TWO_PI, sine_own, cosine_own);
                sine_std   = std::sin(TWO_PI);
                cosine_std = std::cos(TWO_PI);
                accu_sine_delta   += std::fabsf(sine_std - sine_own);
                accu_cosine_delta += std::fabsf(cosine_std - cosine_own);

                const float delta_sin = accu_sine_delta / (check_count + 2);
                const float delta_cos = accu_cosine_delta / (check_count + 2);
                std::printf("[Delta(Sine)]: %f, [Delta(Cosine)]: %f, [#Comp]: %u\n",
                            delta_sin, delta_cos, (check_count + 2));
                REQUIRE((delta_sin < 1e-5 && delta_cos < 1e-5));
            }
        }
        std::printf("--- Checking Combined Sine Cosine: OK!\n");
    } /// SECTION("Checking Combined Sine Cosine")
#endif /// (CHECK_SINE_COSINE == 1)


} /// TEST_CASE("Checking MathUtility", "[MathUtility]")

#endif /// (MATH_UTILITY_CHECKING == 1)
