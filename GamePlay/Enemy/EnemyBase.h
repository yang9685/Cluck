#pragma once

#include "../CharacterActor.h"

class EnemyBase : public CharacterActor
{
public:
    EnemyBase(
        std::unique_ptr<Atlas> InAtlas,
        const Vector2D& InPosition,
        float InRotation,
        int InRenderPriority,
        const Vector2D& InBaseRenderSize,
        float InScale,
        float InFrameIntervalSeconds,
        const Vector2D& InVelocity);

    void Tick(float DeltaTime) override;

protected:
    void Move(float DeltaTime);

protected:
    Vector2D Velocity;
};
