#pragma once

#include <functional>
#include <vector>

#include "../Base/Types.h"
#include "TimerManager.h"

class GameActivityBase;
class EnemyBase;

struct EnemySpawnWeights
{
    double Fast = 1.0;
    double Medium = 3.0;
    double Slow = 5.0;
};

class EnemyManager
{
private:
    EnemyManager() = default;
    ~EnemyManager() = default;

    EnemyManager(const EnemyManager&) = delete;
    EnemyManager& operator=(const EnemyManager&) = delete;

public:
    static EnemyManager& GetInstance()
    {
        static EnemyManager Instance;
        return Instance;
    }

    void Initialize(GameActivityBase& InGameActivity);
    void Shutdown();
    void Update();
    void SetSpawnWeights(const EnemySpawnWeights& InSpawnWeights);
    const EnemySpawnWeights& GetSpawnWeights() const;
    const std::vector<EnemyBase*>& GetEnemies() const;

private:
    void SpawnEnemy();
    void CleanupInvalidEnemies();

private:
    std::reference_wrapper<TimerManager> TimeManager = TimerManager::GetInstance();
    GameActivityBase* GameActivity = nullptr;
    TimerHandle SpawnTimerHandle = InvalidTimerHandle;
    float SpawnIntervalSeconds = 1.0f;
    EnemySpawnWeights SpawnWeights;
    std::vector<EnemyBase*> Enemies;
};
