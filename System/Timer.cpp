#include "Timer.h"
#include <utility>

namespace
{
Uint64 SecondsToMilliseconds(float duration)
{
    if (duration <= 0.0f)
    {
        return 0;
    }

    return static_cast<Uint64>(duration * 1000.0f);
}
}

Timer::Timer(TimerHandle Handle, float DurationSeconds, bool bLoop, TimerCallBack CallBack)
{
    this->Handle = Handle;
    this->DurationSeconds = DurationSeconds;
    this->bLoop = bLoop;
    this->bPaused = false;
    this->CallBack = std::move(CallBack);
    this->RemainingTime = SecondsToMilliseconds(DurationSeconds);
    this->LastTick = 0;
}

void Timer::Execute()
{
    if (bPaused)
    {
        return;
    }

    if (CallBack)
    {
        CallBack();
    }
}

void Timer::Excute()
{
    Execute();
}

void Timer::Reset(Uint64 Now)
{
    RemainingTime = SecondsToMilliseconds(DurationSeconds);
    LastTick = Now;
}

void Timer::Pause(Uint64 Now)
{
    if (!bPaused)
    {
        ConsumeElapsed(Now - LastTick);
        LastTick = Now;
        bPaused = true;
    }
}

void Timer::Resume(Uint64 Now)
{
    LastTick = Now;
    bPaused = false;
}

void Timer::ConsumeElapsed(Uint64 Delta)
{
    if (Delta >= RemainingTime)
    {
        RemainingTime = 0;
    }
    else
    {
        RemainingTime -= Delta;
    }
}

void Timer::SetLastTick(Uint64 Now)
{
    LastTick = Now;
}

TimerHandle Timer::GetHandle() const
{
    return Handle;
}

float Timer::GetDurationSeconds() const
{
    return DurationSeconds;
}

Uint64 Timer::GetRemainingTime() const
{
    return RemainingTime;
}

Uint64 Timer::GetLastTick() const
{
    return LastTick;
}

bool Timer::IsLooping() const
{
    return bLoop;
}

bool Timer::IsPaused() const
{
    return bPaused;
}

bool Timer::HasExpired() const
{
    return RemainingTime == 0;
}
