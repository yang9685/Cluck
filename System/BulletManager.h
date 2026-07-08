#pragma once

#include <vector>

#include "../Base/Vector2D.h"

class Bullet;
class GameActivityBase;

class BulletManager
{
private:
    BulletManager() = default;
    ~BulletManager() = default;

    BulletManager(const BulletManager&) = delete;
    BulletManager& operator=(const BulletManager&) = delete;

public:
    static BulletManager& GetInstance()
    {
        static BulletManager Instance;
        return Instance;
    }

    void Initialize(GameActivityBase& InGameActivity);
    void Shutdown();
    void Update();

    Bullet& CreateBullet(
        const Vector2D& InPosition,
        float InRotation,
        float InSpeed = 800.0f,
        float InScale = 1.0f);
    const std::vector<Bullet*>& GetBullets() const;

private:
    void CleanupInvalidBullets();

private:
    GameActivityBase* GameActivity = nullptr;
    std::vector<Bullet*> Bullets;
};
