#pragma once


/// 游戏逻辑
///
class GameLogic
{
public:
    /// 消息处理(系统，网络...)
    virtual
    void
    process_msg () = 0;

    /// 更新游戏状态
    ///
    /// @param[in] delta_time_ms
    ///      步进的时间ΔT(多少毫秒)
    virtual
    void
    update_level (
        const float delta_time_ms) = 0;

    /// 绘制关卡
    virtual
    void
    draw_level () = 0;
};
