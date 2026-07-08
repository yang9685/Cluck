#include "CollisionManager.h"

#include <unordered_set>

#include <SDL3/SDL_rect.h>

#include "../GamePlay/Bullet.h"
#include "../GamePlay/Enemy/EnemyBase.h"
#include "BulletManager.h"
#include "EnemyManager.h"

void CollisionManager::Update()
{
    const std::vector<Bullet*>& Bullets = BulletManager::GetInstance().GetBullets();
    const std::vector<EnemyBase*>& Enemies = EnemyManager::GetInstance().GetEnemies();
    std::unordered_set<const Bullet*> HitBullets;
    std::unordered_set<const EnemyBase*> HitEnemies;

    for (Bullet* BulletInstance : Bullets)
    {
        if (BulletInstance == nullptr || HitBullets.find(BulletInstance) != HitBullets.end())
        {
            continue;
        }

        for (EnemyBase* EnemyInstance : Enemies)
        {
            if (EnemyInstance == nullptr || HitEnemies.find(EnemyInstance) != HitEnemies.end())
            {
                continue;
            }

            const SDL_FRect& BulletRect = BulletInstance->GetRenderRect();
            const SDL_FRect& EnemyRect = EnemyInstance->GetRenderRect();
            if (!SDL_HasRectIntersectionFloat(&BulletRect, &EnemyRect))
            {
                continue;
            }

            BulletInstance->Destroy();
            EnemyInstance->Die();
            HitBullets.insert(BulletInstance);
            HitEnemies.insert(EnemyInstance);
            break;
        }
    }
}
