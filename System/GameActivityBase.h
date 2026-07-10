#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "Actor.h"
#include "AudioManager.h"
#include "BulletManager.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "PlayerController.h"
#include "RenderManager.h"

enum GameStatus
{
    Success,
    Failed,
    Pending,
};

class GameActivityBase
{
public:
    static GameActivityBase& GetInstance()
    {
        static GameActivityBase Instance;
        return Instance;
    }

    virtual ~GameActivityBase();
    virtual GameStatus GameLoop(float DeltaTime);
    void InitializeRenderManager(SDL_Renderer* Renderer);

    template <typename TActor, typename... TArgs>
    TActor& CreateActor(TArgs&&... Args)
    {
        static_assert(std::is_base_of<Actor, TActor>::value, "TActor must derive from Actor");

        auto NewActor = std::make_unique<TActor>(std::forward<TArgs>(Args)...);
        TActor& ActorRef = *NewActor;
        AddActor(std::move(NewActor));
        return ActorRef;
    }

    virtual void WorldBeginPlay();
    virtual void WorldEndPlay();
    void RequestExit();

    size_t GetActorCount() const;
    const std::vector<std::unique_ptr<Actor>>& GetActors() const;

    void DestroyActor(Actor& ActorInstance);
    void ClearActors();

    float GetMainLoopIntervalSeconds() const;
    void SetMainLoopIntervalSeconds(float IntervalSeconds);

    GameStatus GetCurrentGameStatus() const;
    bool HasWorldBegunPlay() const;

    GameActivityBase(const GameActivityBase&) = delete;
    GameActivityBase& operator=(const GameActivityBase&) = delete;

protected:
    virtual void InitializeSingletons();
    virtual void UpdateSingletons();
    virtual void TickActors(float DeltaTime);
    
    void SortActors();    
    void ProcessPendingDestroyActors();
    bool IsActorPendingDestroy(const Actor& ActorInstance) const;

private:
    GameActivityBase() = default;
    Actor& AddActor(std::unique_ptr<Actor> NewActor);

private:
    GameStatus CurrentGameStatus = Pending;
    std::vector<std::unique_ptr<Actor>> Actors;
    std::vector<Actor*> PendingDestroyActors;
    std::reference_wrapper<AudioManager> AudioManagerInstance = AudioManager::GetInstance();
    std::reference_wrapper<BulletManager> BulletManagerInstance = BulletManager::GetInstance();
    std::reference_wrapper<CollisionManager> CollisionManagerInstance = CollisionManager::GetInstance();
    std::reference_wrapper<EnemyManager> EnemyManagerInstance = EnemyManager::GetInstance();
    std::reference_wrapper<PlayerController> PlayerControllerInstance = PlayerController::GetInstance();
    std::reference_wrapper<RenderManager> RenderManagerInstance = RenderManager::GetInstance();
    float MainLoopIntervalSeconds = 1.0f / 60.0f;
    bool bWorldBegunPlay = false;
};
