/// System headers
#include <cmath> /// std::ceil
/// Library headers
#include "Time/ClockTime.hpp"
#include "Runtime/GameLogic.hpp"
// Self header
#include "Runtime/GameMainLoop.hpp"


#define LOGGING_FRAME_INFO 1


GameMainLoop::GameMainLoop (
    const uint8_t tick_freq,
    GameLogic &   game_logic)
:
    SuperT("== SKY-Dog Game Main Loop =="),
    m_game_logic(game_logic),
    m_tick_time(1000.0f / tick_freq),
    m_delayed_time(0),
    m_last_start_time(0),
    m_fps(tick_freq),
    m_is_running(true),
    m_skipped_draw_count(0),
    m_skip_draw(false)
{
    set_frame_number(1);
    scale_tick_time(1);
}


float
GameMainLoop::fps () const
{
    return m_fps;
}


void
GameMainLoop::scale_tick_time (
    const float scale)
{
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    m_tick_time_scale = scale;
#endif
}


void
GameMainLoop::want_exit ()
{
    m_is_running = false;

    /// 呼醒GameLoop，如果它在休眠
    std::lock_guard<STDMutexT> lock(m_cv_mutex);
    m_condition_var.notify_one();
}


uint32_t
GameMainLoop::run_loop ()
{
    /// 帧流程:
    ///                                                                1到n次
    ///                                                 +-------------+     +------------+
    ///  measure_start_time --> update_delayed_time --> | process_msg | --> | update(dt) | -->
    ///                                                 +-------------+     +------------+
    ///      +------+
    ///  --> | draw | --> measure_end_time --> sleep
    ///      +------+
    ///       0到1次
    ///
    /// 进行跳帧前, 最大的延迟容忍度
    static constexpr uint8_t TOLERANCE_DELAYED_TIME_MS = 8;
    /// 最大可以连续跳帧的次数
    static constexpr uint8_t MAX_SKIPPED_DRAW_COUNT = 5;
    /// 计算FPS时, 对于测量出的FPS的权重
    static constexpr float FRAME_FPS_WEIGHT = 0.2f;

    /// 初始化
    m_last_start_time = ClockTime::mono_time_ms();

    while (m_is_running)
    {
        /// 记录当前Frame的起始时间
        const float frame_start_time = ClockTime::mono_time_ms();

        /// 更新累积的延迟(Delayed Time)
        /// NOTE: 前帧时间(Frame Time)定义为: 这样可以将系统的延迟考虑在内(如Thread调度的时间)
        /// - 从前个Frame开始时刻 -> 当前Frame开始时刻的间隔。
        ///
        ///  / <- Last Frame Time -> \
        /// /                         \
        /// | Last Start              | Frame Start
        /// v                         v
        /// +-----------------------+ +--------------------+
        /// |        FRAME_n        | |      FRAME_n+1     |
        /// +-----------------------+ +--------------------+
        /// Last Frame Time = Frame Start - Last Start
        /// NOTE: 由于我们以Fixed的频率更新GameLoop, 所有每一个Frame Time >= Tick Time
        ///
        const float last_frame_time = frame_start_time - m_last_start_time;

        /// 计算累积的延迟
        m_delayed_time += last_frame_time;

        /// 持续更新, 直到剩余时间小于一个Tick Time
        /// NOTE: 由于Frame Time >= Tick Time, 所以我们在这里至少更新一次
        while (m_is_running && (m_delayed_time >= m_tick_time))
        {
            m_game_logic.process_msg();

            /// 以固定ΔT更新
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            m_game_logic.update_level(m_tick_time * m_tick_time_scale);
#else
            m_game_logic.update_level(m_tick_time);
#endif

            /// 消耗一个Tick Time
            m_delayed_time -= m_tick_time;
        }

        /// 对上帧确定的跳帧标记进行判断
        /// 来确定当前Frame是否忽略DRAW
        if (m_skip_draw)
        {
            if ((m_skipped_draw_count + 1) >= MAX_SKIPPED_DRAW_COUNT)
            {
                /// 连续跳帧过多，绘制一次
                m_skip_draw = false;
                m_skipped_draw_count = 0;
            }
            else
            {
                ++m_skipped_draw_count;
            }
        }

        if (m_is_running && (m_skip_draw == false))
        {
            m_game_logic.draw_level();
            increase_frame_number();
        }

        /// 确定是否下个Frame跳帧
        const float frame_end_time  = ClockTime::mono_time_ms();
        const float frame_used_time = frame_end_time - frame_start_time;
        /// 添加一个容忍区域: 在这个区域, 我们不跳帧
        m_skip_draw = frame_used_time > (m_tick_time + TOLERANCE_DELAYED_TIME_MS);

        /// 使用上一帧的时间, 计算上一帧的FPS
        const float last_fps = 1000.0f / last_frame_time;
        /// 更新FPS
        m_fps = (1 - FRAME_FPS_WEIGHT) * m_fps + FRAME_FPS_WEIGHT * last_fps;

        /// 更新时间戳
        m_last_start_time = frame_start_time;

        /// 如有剩余时间, 休眠
        const float sleep_time_ms = m_tick_time - frame_used_time;
        std::unique_lock<STDMutexT> lock(m_cv_mutex);
        m_condition_var.wait_for(
            lock,
            std::chrono::milliseconds((int64_t)(std::ceil(sleep_time_ms))),
            [this] { return m_is_running == false; }); /// Predicate为TRUE, wait_for()退出
    }

    return 0;
}


void
GameMainLoop::increase_frame_number()
{
    set_frame_number(get_frame_number() + 1);
}


uint64_t
GameMainLoop::get_frame_number() const
{
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    return m_frame_number;
#else
    return 0;
#endif
}


void
GameMainLoop::set_frame_number (
    const uint64_t new_number)
{
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    m_frame_number = new_number;
#endif
}
