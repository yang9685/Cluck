#include "SlowChickenEnemy.h"

#include "../../Resource/ResourcePaths.h"

SlowChickenEnemy::SlowChickenEnemy(const Vector2D& InPosition, float InRotation)
    : EnemyBase(
        CreateAtlasFromPaths(RenderManager::GetInstance().GetRenderer(), Resources::Image::ChickenSlow),
        InPosition,
        InRotation,
        10,
        Vector2D(32.0f, 52.0f),
        1.5f,
        0.1f,
        Vector2D(0.0f, 70.0f))
{
}
