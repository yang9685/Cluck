#pragma once
#include <SDL3/SDL_stdinc.h>
#include "../Base/Types.h"

class Timer
{
public:
    Timer(TimerHandle Handle, float DurationSeconds, bool bLoop, TimerCallBack CallBack = {});

    virtual ~Timer() = default;

    virtual void Execute();
    virtual void Excute();
    void Reset(Uint64 Now);
    void Pause(Uint64 Now);
    void Resume(Uint64 Now);
    void ConsumeElapsed(Uint64 Delta);
    void SetLastTick(Uint64 Now);

    TimerHandle GetHandle() const;
    float GetDurationSeconds() const;
    Uint64 GetRemainingTime() const;
    Uint64 GetLastTick() const;
    bool IsLooping() const;
    bool IsPaused() const;
    bool HasExpired() const;

private:
    TimerHandle Handle;
    float DurationSeconds;
    Uint64 RemainingTime;
    Uint64 LastTick;
    bool bLoop;
    bool bPaused;
    TimerCallBack CallBack;
};
