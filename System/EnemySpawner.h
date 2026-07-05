#pragma once

#include <functional>

#include "../Base/Types.h"
#include "TimerManager.h"

class GameActivityBase;

struct EnemySpawnWeights
{
    double Fast = 1.0;
    double Medium = 3.0;
    double Slow = 5.0;
};

class EnemySpawner
{
private:
    EnemySpawner() = default;
    ~EnemySpawner() = default;

    EnemySpawner(const EnemySpawner&) = delete;
    EnemySpawner& operator=(const EnemySpawner&) = delete;

public:
    static EnemySpawner& GetInstance()
    {
        static EnemySpawner Instance;
        return Instance;
    }

    void Initialize(GameActivityBase& InGameActivity);
    void Shutdown();
    void SetSpawnWeights(const EnemySpawnWeights& InSpawnWeights);
    const EnemySpawnWeights& GetSpawnWeights() const;

private:
    void SpawnEnemy();

private:
    std::reference_wrapper<TimerManager> TimeManager = TimerManager::GetInstance();
    GameActivityBase* GameActivity = nullptr;
    TimerHandle SpawnTimerHandle = InvalidTimerHandle;
    float SpawnIntervalSeconds = 1.0f;
    EnemySpawnWeights SpawnWeights;
};
