#pragma once
#include <map>
#include <memory>
#include <SDL3/SDL_stdinc.h>
#include "Timer.h"
#include "../Base/Types.h"

class TimerManager
{
private:
    TimerManager() = default;
    ~TimerManager() = default;

    TimerManager(const TimerManager&) = delete;
    TimerManager& operator=(const TimerManager&) = delete;

public:
    static TimerManager& GetInstance()
    {
        static TimerManager instance;
        return instance;
    }

    TimerHandle CreateTimer(float duration, bool bloop, TimerCallBack Callback);
    bool DestroyTimer(TimerHandle Handle);
    bool DestoryTimer(TimerHandle Handle);
    bool StopTimer(TimerHandle Handle);
    bool StartTimer(TimerHandle Handle);
    void Update();

private:
    std::map<TimerHandle, std::unique_ptr<Timer>> timers;
    TimerHandle NextID = InvalidTimerHandle + 1;
};
