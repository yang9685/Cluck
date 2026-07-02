#include "TimerManager.h"

#include <utility>
#include <vector>
#include <SDL3/SDL_timer.h>

namespace
{
bool IsValidHandle(TimerHandle Handle)
{
    return Handle != InvalidTimerHandle;
}
}

TimerHandle TimerManager::CreateTimer(float duration, bool bloop, TimerCallBack Callback)
{
    if (NextID == InvalidTimerHandle)
    {
        ++NextID;
    }

    TimerHandle NewTimerHandle = NextID++;
    Uint64 Now = SDL_GetTicks();

    auto NewTimer = std::make_unique<Timer>(NewTimerHandle, duration, bloop, std::move(Callback));
    NewTimer->SetLastTick(Now);

    timers.emplace(NewTimerHandle, std::move(NewTimer));
    return NewTimerHandle;
}

bool TimerManager::DestroyTimer(TimerHandle Handle)
{
    if (!IsValidHandle(Handle))
    {
        return false;
    }

    auto Timer = timers.find(Handle);
    if (Timer == timers.end())
    {
        return false;
    }

    timers.erase(Timer);
    return true;
}

bool TimerManager::DestoryTimer(TimerHandle Handle)
{
    return DestroyTimer(Handle);
}

bool TimerManager::StopTimer(TimerHandle Handle)
{
    if (!IsValidHandle(Handle))
    {
        return false;
    }

    auto Timer = timers.find(Handle);
    if (Timer == timers.end())
    {
        return false;
    }

    Timer->second->Pause(SDL_GetTicks());

    return true;
}

bool TimerManager::StartTimer(TimerHandle Handle)
{
    if (!IsValidHandle(Handle))
    {
        return false;
    }

    auto Timer = timers.find(Handle);
    if (Timer == timers.end())
    {
        return false;
    }

    Timer->second->Resume(SDL_GetTicks());
    return true;
}

void TimerManager::Update()
{
    std::vector<TimerHandle> TimerHandles;
    std::vector<TimerHandle> PendingTimers;

    TimerHandles.reserve(timers.size());
    PendingTimers.reserve(timers.size());

    for (const auto& TimerPair : timers)
    {
        TimerHandles.push_back(TimerPair.first);
    }

    for (TimerHandle Handle : TimerHandles)
    {
        auto Timer = timers.find(Handle);
        if (Timer == timers.end() || Timer->second->IsPaused())
        {
            continue;
        }

        Uint64 CurrentNow = SDL_GetTicks();
        Uint64 Delta = CurrentNow - Timer->second->GetLastTick();
        Timer->second->SetLastTick(CurrentNow);

        if (Delta < Timer->second->GetRemainingTime())
        {
            Timer->second->ConsumeElapsed(Delta);
            continue;
        }

        const bool bLoop = Timer->second->IsLooping();
        Timer->second->ConsumeElapsed(Delta);
        Timer->second->Execute();

        Timer = timers.find(Handle);
        if (Timer == timers.end())
        {
            continue;
        }

        if (bLoop)
        {
            Timer->second->Reset(SDL_GetTicks());
        }
        else
        {
            PendingTimers.push_back(Handle);
        }
    }

    for (TimerHandle Handle : PendingTimers)
    {
        auto Timer = timers.find(Handle);
        if (Timer != timers.end())
        {
            timers.erase(Timer);
        }
    }
}
