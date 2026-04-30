#include "External/Catch/catch_v2.hpp"
#include "UT_Config.hpp"

#if (GAME_LOOP_CHECKING == 1)
#include <cstdio> /// std::printf
#include <chrono> /// milliseconds
#include <thread> /// sleep_for
#include "Runtime/GameLogic.hpp"
#include "Runtime/GameMainLoop.hpp"


class TestLogic : public GameLogic
{
    /// 消息处理(系统，网络...)
    virtual
    void
    process_msg () override
    {}

    /// 更新游戏状态
    ///
    /// @param[in] delta_time_ms
    ///      步进的时间ΔT(多少毫秒)
    virtual
    void
    update_level (
        const float delta_time_ms) override
    {
        std::printf("[Update]: dt(%fms)\n", delta_time_ms);
    }

    /// 绘制关卡
    virtual
    void
    draw_level () override
    {
        std::printf("[Draw]\n");
    }
};


/// TEST_CASE( name [, tags ] )
TEST_CASE("Checking GameLoop", "[GameLoop]")
{
    TestLogic test_logic;
    GameMainLoop game_loop(30, test_logic);

    SECTION("Checking GameLoop")
    {
        std::printf("--- Checking GameLoop...\n");

        game_loop.start();

        std::this_thread::sleep_for(std::chrono::milliseconds(10 * 1000));

        game_loop.want_exit();
        game_loop.wait_for_exit();

        //REQUIRE((isSuccess));
        std::printf("--- Checking GameLoop: OK!\n");
    } /// SECTION("Checking GameLoop")

} /// TEST_CASE("Checking GameLoop", "[GameLoop]")

#endif /// (GAME_LOOP_CHECKING == 1)
