#include "GameActivityBase.h"

#include <algorithm>
#include <stdexcept>

GameActivityBase::~GameActivityBase()
{
    WorldEndPlay();
}

GameStatus GameActivityBase::GameLoop()
{
    return Update();
}

Actor& GameActivityBase::AddActor(std::unique_ptr<Actor> NewActor)
{
    if (!NewActor)
    {
        throw std::invalid_argument("Cannot add a null actor");
    }

    Actor& ActorRef = *NewActor;
    Actors.push_back(std::move(NewActor));

    if (bWorldBegunPlay)
    {
        ActorRef.BeginPlay();
    }

    return ActorRef;
}

void GameActivityBase::InitializeSingletons()
{
}

void GameActivityBase::UpdateSingletons()
{
    TimeManager.get().Update();
}

void GameActivityBase::TickActors(float DeltaTime)
{
    for (const std::unique_ptr<Actor>& ActorInstance : Actors)
    {
        if (ActorInstance && !IsActorPendingDestroy(*ActorInstance))
        {
            ActorInstance->Tick(DeltaTime);
        }
    }
}

void GameActivityBase::WorldBeginPlay()
{
    if (bWorldBegunPlay)
    {
        return;
    }

    InitializeSingletons();

    if (MainLoopTimerHandle == InvalidTimerHandle)
    {
        MainLoopTimerHandle = TimeManager.get().CreateTimer(
            MainLoopIntervalSeconds,
            true,
            [this]()
            {
                TickActors(MainLoopIntervalSeconds);
            });
    }

    for (const std::unique_ptr<Actor>& ActorInstance : Actors)
    {
        if (ActorInstance)
        {
            ActorInstance->BeginPlay();
        }
    }

    bWorldBegunPlay = true;
}

void GameActivityBase::WorldEndPlay()
{
    if (MainLoopTimerHandle != InvalidTimerHandle)
    {
        TimeManager.get().DestroyTimer(MainLoopTimerHandle);
        MainLoopTimerHandle = InvalidTimerHandle;
    }

    bWorldBegunPlay = false;
}

GameStatus GameActivityBase::Update()
{
    UpdateSingletons();
    ProcessPendingDestroyActors();
    return CurrentGameStatus;
}

size_t GameActivityBase::GetActorCount() const
{
    return Actors.size();
}

const std::vector<std::unique_ptr<Actor>>& GameActivityBase::GetActors() const
{
    return Actors;
}

void GameActivityBase::DestroyActor(Actor& ActorInstance)
{
    if (IsActorPendingDestroy(ActorInstance))
    {
        return;
    }

    const auto ActorIt = std::find_if(
        Actors.begin(),
        Actors.end(),
        [&ActorInstance](const std::unique_ptr<Actor>& CurrentActor)
        {
            return CurrentActor.get() == &ActorInstance;
        });

    if (ActorIt != Actors.end())
    {
        PendingDestroyActors.push_back(&ActorInstance);
    }
}

void GameActivityBase::ClearActors()
{
    Actors.clear();
    PendingDestroyActors.clear();
}

TimerManager& GameActivityBase::GetTimeManager() const
{
    return TimeManager.get();
}

float GameActivityBase::GetMainLoopIntervalSeconds() const
{
    return MainLoopIntervalSeconds;
}

void GameActivityBase::SetMainLoopIntervalSeconds(float IntervalSeconds)
{
    MainLoopIntervalSeconds = IntervalSeconds;
}

TimerHandle GameActivityBase::GetMainLoopTimerHandle() const
{
    return MainLoopTimerHandle;
}

void GameActivityBase::SetMainLoopTimerHandle(TimerHandle Handle)
{
    MainLoopTimerHandle = Handle;
}

GameStatus GameActivityBase::GetCurrentGameStatus() const
{
    return CurrentGameStatus;
}

void GameActivityBase::SetCurrentGameStatus(GameStatus NewStatus)
{
    CurrentGameStatus = NewStatus;
}

bool GameActivityBase::HasWorldBegunPlay() const
{
    return bWorldBegunPlay;
}

void GameActivityBase::ProcessPendingDestroyActors()
{
    if (PendingDestroyActors.empty())
    {
        return;
    }

    Actors.erase(
        std::remove_if(
            Actors.begin(),
            Actors.end(),
            [this](const std::unique_ptr<Actor>& ActorInstance)
            {
                return ActorInstance && IsActorPendingDestroy(*ActorInstance);
            }),
        Actors.end());

    PendingDestroyActors.clear();
}

bool GameActivityBase::IsActorPendingDestroy(const Actor& ActorInstance) const
{
    return std::find(
               PendingDestroyActors.begin(),
               PendingDestroyActors.end(),
               &ActorInstance) != PendingDestroyActors.end();
}
