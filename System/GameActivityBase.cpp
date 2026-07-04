#include "GameActivityBase.h"

#include <algorithm>
#include <stdexcept>

GameActivityBase::~GameActivityBase()
{
    WorldEndPlay();
}

GameStatus GameActivityBase::GameLoop(float DeltaTime)
{
    if (!bWorldBegunPlay)
    {
        return CurrentGameStatus;
    }
    SortActors();
    UpdateSingletons();
    TickActors(DeltaTime);
    ProcessPendingDestroyActors();
    return CurrentGameStatus;
}

void GameActivityBase::InitializeRenderManager(SDL_Renderer* Renderer)
{
    RenderManagerInstance.get().Initialize(Renderer);
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
    RenderManagerInstance.get().Update(Actors);
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

void GameActivityBase::SortActors()
{
    if (Actors.size() > 0)
    {
        std::sort(Actors.begin(), Actors.end(), [](const std::unique_ptr<Actor>& A, const std::unique_ptr<Actor>& B)
        {
            return A->RenderPriority < B->RenderPriority;
        }); 
    }
}

void GameActivityBase::WorldBeginPlay()
{
    if (bWorldBegunPlay)
    {
        return;
    }

    InitializeSingletons();

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
    bWorldBegunPlay = false;
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

float GameActivityBase::GetMainLoopIntervalSeconds() const
{
    return MainLoopIntervalSeconds;
}

void GameActivityBase::SetMainLoopIntervalSeconds(float IntervalSeconds)
{
    MainLoopIntervalSeconds = IntervalSeconds;
}

GameStatus GameActivityBase::GetCurrentGameStatus() const
{
    return CurrentGameStatus;
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
