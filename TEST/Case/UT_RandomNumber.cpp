#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (RANDOM_NUMBER_CHECKING == 1)
/// System headers
#include <cstdlib> /// std::rand
/// Libary headers
#include "Math/RandomNumber.hpp"
#include "Time/ClockTime.hpp"


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking Random number", "[Random number]")
{
    /// 测试十亿次
    static constexpr uint32_t ITERATION_COUNT = 1000000000;

    RandomNumber rand_num_gen;
    std::srand((unsigned int)ClockTime::mono_time_ms());

    SECTION("Checking Random number Speed:")
    {
        std::printf("--- Checking Random number Speed...\n");
        std::printf("Generating %u numbers...\n", ITERATION_COUNT);
        {
            int rand_num = 0;

            const float start_time = ClockTime::mono_time_ms();
            for (uint32_t c = 0; c < ITERATION_COUNT; ++c)
            {
                /// NOTE: std::rand()函数返回[0, RAND_MAX], RAND_MAX定义位0x7FFF
                rand_num = std::rand();
            }

            const float end_time = ClockTime::mono_time_ms();
            const float used_time_own = end_time - start_time;
            /// 我们不得不输出随机数_rand_num，来避免随机数发生器调用被优化掉!
            std::printf("[STD Rand]: %.4fms, %.8fs, [Dummy]: %u\n",
                        used_time_own, used_time_own / 1000, rand_num);
        }

        {
            uint64_t rand_num = 0;
            const float start_time = ClockTime::mono_time_ms();
            for (uint32_t c = 0; c < ITERATION_COUNT; ++c)
            {
                rand_num = rand_num_gen.next_uint();
            }

            const float end_time = ClockTime::mono_time_ms();
            const float used_time_own = end_time - start_time;
            std::printf("[Own Rand]: %.4fms, %.8fs, [Dummy]: %llu\n",
                        used_time_own, used_time_own / 1000, rand_num);
        }

        std::printf("--- Checking Random number Speed: OK!\n");
    }


    SECTION("Checking Random number distribution:")
    {
        /// 测试桶数目
        static constexpr uint32_t BUCKET_COUNT = 256;
        /// 测试的自由度: degree of freedom
        static constexpr uint32_t FREEDOM_DEGREE = BUCKET_COUNT - 1;
        /// 每桶期待的数值数目
        static const float EXPECTED_COUNT = (float)ITERATION_COUNT / BUCKET_COUNT;

        /// 检测随机数分布
        std::printf("--- Checking Random number distribution...\n");
        std::printf("Generating %u numbers into %u buckets...\n",
                    ITERATION_COUNT, BUCKET_COUNT);

        {
            /// 每桶中数值的个数
            uint64_t rand_num_counts[BUCKET_COUNT]{ 0 };

            for (uint32_t i = 0; i < ITERATION_COUNT; ++i)
            {
                /// 计算桶索引
                const uint64_t bucket_idx = std::rand() % BUCKET_COUNT;
                ++rand_num_counts[bucket_idx];
            }

            /// χ² := (每桶测量数值数目 - 每桶期待数值数目)² / 每桶期待数值数目
            float chi2 = 0;
            for (uint32_t i = 0; i < BUCKET_COUNT; ++i)
            {
                const float dif = (float)rand_num_counts[i] - EXPECTED_COUNT;
                chi2 += dif * dif / EXPECTED_COUNT;
            }

            /// 计算Z-Score:分布中的波动为几个偏差大小:
            /// Z = (χ² - df) / √(2df)
            /// df为degree of freedom
            const float z_score = (chi2 - FREEDOM_DEGREE) /
                                  std::sqrt(2.0f * FREEDOM_DEGREE);
            std::printf("[Z-Score(STD)(0 ~ 3)]: %.2f, [Chi^2(STD)]: %.2f\n",
                        z_score, chi2);
        }


        {
            /// 每桶中数值的个数
            uint64_t rand_num_counts[BUCKET_COUNT]{ 0 };

            for (uint32_t i = 0; i < ITERATION_COUNT; ++i)
            {
                /// const uint64_t bucket_idx =
                ///     rand_num_gen.next_uint_range(0, BIN_COUNT - 1);
                const uint64_t bucket_idx = rand_num_gen.next_uint() % BUCKET_COUNT;
                ++rand_num_counts[bucket_idx];
            }

            /// 计算偏差χ:
            /// χ² := (每桶测量数值数目 - 每桶期待数值数目)² / 每桶期待数值数目
            float chi2 = 0;
            for (uint32_t i = 0; i < BUCKET_COUNT; ++i)
            {
                const float dif = (float)rand_num_counts[i] - EXPECTED_COUNT;
                chi2 += dif * dif / EXPECTED_COUNT;
            }

            /// 计算Z-Score:分布中的波动为几个偏差大小:
            /// Z = (χ² - df) / √(2df)
            /// df为degree of freedom
            const float z_score = (chi2 - FREEDOM_DEGREE) /
                                  std::sqrt(2.0f * FREEDOM_DEGREE);
            std::printf("[Z-Score(Own)(0 ~ 3)]: %.2f, [Chi^2(Own)]: %.2f\n",
                        z_score, chi2);
        }

///        {
///            std::printf("---------- Random number map ----------\n");
///            /// 以生成的随机数为坐标, 生成一张图片
///            uint64_t rand_num_count_map[BUCKET_COUNT][BUCKET_COUNT]{ 0 };
///            for (uint32_t i = 0; i < BUCKET_COUNT; ++i)
///            {
///                for (uint32_t j = 0; j < BUCKET_COUNT; ++j)
///                {
///                    const uint64_t col_idx =
///                    rand_num_gen.next_uint_range(0, BUCKET_COUNT - 1);
///                    const uint64_t row_idx =
///                    rand_num_gen.next_uint_range(0, BUCKET_COUNT - 1);
///                    ++rand_num_count_map[row_idx][col_idx];
///
///                    std::printf("<%llu, %llu>\n", row_idx, col_idx);
///                }
///            }
///        }

        std::printf("--- Checking Random number distribution: OK!\n");
    }

} /// TEST_CASE("Checking Random number", "[Random numberr]")

#endif /// (RANDOM_NUMBER_CHECKING == 1)
