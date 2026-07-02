#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "Actor.h"
#include "TimerManager.h"
#include "../Base/Types.h"

enum GameStatus
{
    Success,
    Failed,
    Pending,
};

//游戏世界基类
class GameActivityBase
{
public:
    virtual GameStatus GameLoop();

    virtual ~GameActivityBase();

    Actor& AddActor(std::unique_ptr<Actor> NewActor);

protected:
    TimerHandle MainLoopTimerHandle = InvalidTimerHandle;
    GameStatus CurrentGameStatus = Pending;
    std::vector<std::unique_ptr<Actor>> Actors;
    std::vector<Actor*> PendingDestroyActors;
    std::reference_wrapper<TimerManager> TimeManager = TimerManager::GetInstance();
    float MainLoopIntervalSeconds = 1.0f / 60.0f;
    bool bWorldBegunPlay = false;

public:
    virtual void WorldBeginPlay();
    virtual void WorldEndPlay();
    virtual GameStatus Update();

    size_t GetActorCount() const;
    const std::vector<std::unique_ptr<Actor>>& GetActors() const;

    void DestroyActor(Actor& ActorInstance);
    void ClearActors();

    TimerManager& GetTimeManager() const;

    float GetMainLoopIntervalSeconds() const;
    void SetMainLoopIntervalSeconds(float IntervalSeconds);
    
    TimerHandle GetMainLoopTimerHandle() const;
    void SetMainLoopTimerHandle(TimerHandle Handle);

    GameStatus GetCurrentGameStatus() const;
    void SetCurrentGameStatus(GameStatus NewStatus);

    bool HasWorldBegunPlay() const;

protected:
    virtual void InitializeSingletons();
    virtual void UpdateSingletons();
    virtual void TickActors(float DeltaTime);
    void ProcessPendingDestroyActors();
    bool IsActorPendingDestroy(const Actor& ActorInstance) const;
};
