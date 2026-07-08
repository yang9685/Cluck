#include "EnemyManager.h"

#include <array>
#include <algorithm>
#include <random>

#include "../GamePlay/Enemy/EnemyBase.h"
#include "../GamePlay/Enemy/FastChickenEnemy.h"
#include "../GamePlay/Enemy/MediumChickenEnemy.h"
#include "../GamePlay/Enemy/SlowChickenEnemy.h"
#include "GameActivityBase.h"

void EnemyManager::Initialize(GameActivityBase& InGameActivity)
{
    Shutdown();

    GameActivity = &InGameActivity;
    Enemies.clear();
    SpawnTimerHandle = TimeManager.get().CreateTimer(
        SpawnIntervalSeconds,
        true,
        [this]()
        {
            SpawnEnemy();
        });
}

void EnemyManager::Shutdown()
{
    if (SpawnTimerHandle != InvalidTimerHandle)
    {
        TimeManager.get().DestroyTimer(SpawnTimerHandle);
        SpawnTimerHandle = InvalidTimerHandle;
    }

    Enemies.clear();
    GameActivity = nullptr;
}

void EnemyManager::Update()
{
    CleanupInvalidEnemies();
}

void EnemyManager::SetSpawnWeights(const EnemySpawnWeights& InSpawnWeights)
{
    SpawnWeights = InSpawnWeights;
}

const EnemySpawnWeights& EnemyManager::GetSpawnWeights() const
{
    return SpawnWeights;
}

const std::vector<EnemyBase*>& EnemyManager::GetEnemies() const
{
    return Enemies;
}

void EnemyManager::SpawnEnemy()
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
        EnemyBase& NewEnemy = GameActivity->CreateActor<MediumChickenEnemy>(SpawnPosition, 0.0f);
        Enemies.push_back(&NewEnemy);
        return;
    }

    std::discrete_distribution<int> EnemyTypeDistribution(
        SpawnWeightValues.begin(),
        SpawnWeightValues.end());

    switch (EnemyTypeDistribution(RandomEngine))
    {
    case 0:
        Enemies.push_back(&GameActivity->CreateActor<FastChickenEnemy>(SpawnPosition, 0.0f));
        break;
    case 1:
        Enemies.push_back(&GameActivity->CreateActor<MediumChickenEnemy>(SpawnPosition, 0.0f));
        break;
    default:
        Enemies.push_back(&GameActivity->CreateActor<SlowChickenEnemy>(SpawnPosition, 0.0f));
        break;
    }
}

void EnemyManager::CleanupInvalidEnemies()
{
    if (GameActivity == nullptr)
    {
        Enemies.clear();
        return;
    }

    const std::vector<std::unique_ptr<Actor>>& Actors = GameActivity->GetActors();
    Enemies.erase(
        std::remove_if(
            Enemies.begin(),
            Enemies.end(),
            [&Actors](const EnemyBase* EnemyInstance)
            {
                return EnemyInstance == nullptr || std::none_of(
                    Actors.begin(),
                    Actors.end(),
                    [EnemyInstance](const std::unique_ptr<Actor>& ActorInstance)
                    {
                        return ActorInstance.get() == EnemyInstance;
                    });
            }),
        Enemies.end());
}
