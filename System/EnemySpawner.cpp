#include "EnemySpawner.h"

#include <array>
#include <random>

#include "../GamePlay/Enemy/FastChickenEnemy.h"
#include "../GamePlay/Enemy/MediumChickenEnemy.h"
#include "../GamePlay/Enemy/SlowChickenEnemy.h"
#include "GameActivityBase.h"

void EnemySpawner::Initialize(GameActivityBase& InGameActivity)
{
    Shutdown();

    GameActivity = &InGameActivity;
    SpawnTimerHandle = TimeManager.get().CreateTimer(
        SpawnIntervalSeconds,
        true,
        [this]()
        {
            SpawnEnemy();
        });
}

void EnemySpawner::Shutdown()
{
    if (SpawnTimerHandle != InvalidTimerHandle)
    {
        TimeManager.get().DestroyTimer(SpawnTimerHandle);
        SpawnTimerHandle = InvalidTimerHandle;
    }

    GameActivity = nullptr;
}

void EnemySpawner::SetSpawnWeights(const EnemySpawnWeights& InSpawnWeights)
{
    SpawnWeights = InSpawnWeights;
}

const EnemySpawnWeights& EnemySpawner::GetSpawnWeights() const
{
    return SpawnWeights;
}

void EnemySpawner::SpawnEnemy()
{
    if (GameActivity == nullptr)
    {
        return;
    }

    static std::mt19937 RandomEngine(std::random_device{}());
    std::uniform_real_distribution<float> SpawnXDistribution(100.0f, 1820.0f);
    const Vector2D SpawnPosition(SpawnXDistribution(RandomEngine), -100.0f);
    const std::array<double, 3> SpawnWeightValues = {
        SpawnWeights.Fast,
        SpawnWeights.Medium,
        SpawnWeights.Slow,
    };

    bool bHasPositiveWeight = false;
    for (double WeightValue : SpawnWeightValues)
    {
        if (WeightValue > 0.0)
        {
            bHasPositiveWeight = true;
            break;
        }
    }

    if (!bHasPositiveWeight)
    {
        GameActivity->CreateActor<MediumChickenEnemy>(SpawnPosition, 0.0f);
        return;
    }

    std::discrete_distribution<int> EnemyTypeDistribution(
        SpawnWeightValues.begin(),
        SpawnWeightValues.end());

    switch (EnemyTypeDistribution(RandomEngine))
    {
    case 0:
        GameActivity->CreateActor<FastChickenEnemy>(SpawnPosition, 0.0f);
        break;
    case 1:
        GameActivity->CreateActor<MediumChickenEnemy>(SpawnPosition, 0.0f);
        break;
    default:
        GameActivity->CreateActor<SlowChickenEnemy>(SpawnPosition, 0.0f);
        break;
    }
}
