#pragma once


// System headers
#include <atomic>
#include <condition_variable>
#include <mutex>
// Lib headers
#include "Common/CompilerDefines.hpp" /// BUILD_MODE
#include "DataType/BuiltInTypes.hpp"
#include "Thread/WorkerThread.hpp"


class GameLogic;


class GameMainLoop : public WorkerThread
{
public:
    /// 创建一个游戏主循环
    ///
    /// @param[in]  tick_freq
    ///     游戏世界中Tick的频率(例如30赫兹): 即, 定义多长时间我们要进行一次世界更新
    /// @param[in]  game_logic
    ///     游戏逻辑实例
    explicit
    GameMainLoop (
        const UByte tick_freq,
        GameLogic & game_logic);

    /// 获得帧率
    Float
    fps () const;

    /// 放缩Tick Time
    void
    scale_tick_time (
        const Float scale);

    /// 通知MainLoop准备退出
    void
    want_exit ();


protected:
    /// 此线程的真正任务所在
    /// NOTE：子类必须提供具体的功能
    ///
    /// @return
    ///     线程的返回值
    ///     -0: 成功退出
    ///     -ERROR_EXIT_CODE: 错误退出
    virtual
    UInt
    run_loop () override;

private:
    void
    increase_frame_number ();

    ULong
    get_frame_number () const;

    void
    set_frame_number (
        const ULong new_number);

private:
    typedef WorkerThread SuperT;
    typedef std::mutex STDMutexT;
    typedef std::condition_variable ConditionVarT;
    typedef std::atomic<Bool> AtomicBoolT;

    /// 游戏逻辑
    GameLogic &   m_game_logic;
    /// 游戏时间/Tick时间(多少毫秒): 即, 每个Tick之间的时间间隔
    const Float   m_tick_time;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    /// 帧的号码(1开始)
    ULong         m_frame_number;
    /// Tick时间的放缩系数
    Float         m_tick_time_scale;
#endif
    /// 累积的游戏时间延迟(多少毫秒): 即, 相对于Tick时间的延迟
    Float         m_delayed_time;
    /// 前个Frame的起始时间(多少毫秒)
    Float         m_last_start_time;
    Float         m_fps;
    STDMutexT     m_cv_mutex;
    ConditionVarT m_condition_var;
    /// 运行标记
    AtomicBoolT   m_is_running;
    /// 连续跳帧的次数
    UByte         m_skipped_draw_count;
    /// 是否跳帧: 即,是否跳过绘制(DRAW)操作
    Bool          m_skip_draw;
};
