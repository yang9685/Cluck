#include "BulletManager.h"

#include <algorithm>

#include "../GamePlay/Bullet.h"
#include "GameActivityBase.h"

void BulletManager::Initialize(GameActivityBase& InGameActivity)
{
    GameActivity = &InGameActivity;
    Bullets.clear();
}

void BulletManager::Shutdown()
{
    Bullets.clear();
    GameActivity = nullptr;
}

void BulletManager::Update()
{
    CleanupInvalidBullets();
}

Bullet& BulletManager::CreateBullet(const Vector2D& InPosition, float InRotation, float InSpeed, float InScale)
{
    Bullet& NewBullet = GameActivity->CreateActor<Bullet>(InPosition, InRotation, InSpeed, InScale);
    Bullets.push_back(&NewBullet);
    return NewBullet;
}

const std::vector<Bullet*>& BulletManager::GetBullets() const
{
    return Bullets;
}

void BulletManager::CleanupInvalidBullets()
{
    if (GameActivity == nullptr)
    {
        Bullets.clear();
        return;
    }

    const std::vector<std::unique_ptr<Actor>>& Actors = GameActivity->GetActors();
    Bullets.erase(
        std::remove_if(
            Bullets.begin(),
            Bullets.end(),
            [&Actors](const Bullet* BulletInstance)
            {
                return BulletInstance == nullptr || std::none_of(
                    Actors.begin(),
                    Actors.end(),
                    [BulletInstance](const std::unique_ptr<Actor>& ActorInstance)
                    {
                        return ActorInstance.get() == BulletInstance;
                    });
            }),
        Bullets.end());
}
