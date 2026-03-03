#include "Common/PlatformDefines.hpp"
/// System headers
#if (OS_TYPE == OS_TYPE_WIN)
/// 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#define _WINSOCKAPI_
#include <Windows.h>
#undef _WINSOCKAPI_
#elif defined(__APPLE__)
#include <time.h> /// clock_gettime_nsec_np
#endif
/// Self header
#include "Time/ClockTime.hpp"


Float
ClockTime::mono_time_ms ()
{
#if (OS_TYPE == OS_TYPE_WIN)
    /// 获取Performance计数器(以)的频率
    static LARGE_INTEGER COUNTER_FREQUENCY{0};

    if (COUNTER_FREQUENCY.QuadPart == 0)
    {
        QueryPerformanceFrequency(&COUNTER_FREQUENCY);
    }

    /// 获取当前Performance计数器的数值(通常小于1微秒: < 1us)
    LARGE_INTEGER counter_val;
    QueryPerformanceCounter(&counter_val);

    /// 计算当前时间(多少秒)
    const Double time_in_sec = (Double)counter_val.QuadPart / COUNTER_FREQUENCY.QuadPart;

    /// 讲秒转换为毫秒
    return time_in_sec * 1000;

#elif defined(__APPLE__)
    /// 获得纳秒为单位的Monotonic时间
    /// NOTE: CLOCK_UPTIME_RAW不包含系统sleep的时间
    const ULong time_in_ns = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);

    /// 将时间从纳秒转换到毫秒: 1ms = 1000000纳秒
    return time_in_ns * (1.0f/1000000);

#else /// Linux
    #error TODO: No implementation
#endif /// (OS_TYPE == OS_TYPE_WIN)
}
