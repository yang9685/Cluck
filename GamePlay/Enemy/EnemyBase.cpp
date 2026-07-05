#include "EnemyBase.h"

EnemyBase::EnemyBase(
    std::unique_ptr<Atlas> InAtlas,
    const Vector2D& InPosition,
    float InRotation,
    int InRenderPriority,
    const Vector2D& InBaseRenderSize,
    float InScale,
    float InFrameIntervalSeconds,
    const Vector2D& InVelocity)
    : CharacterActor(
        std::move(InAtlas),
        InPosition,
        InRotation,
        InRenderPriority,
        InBaseRenderSize,
        InScale,
        InFrameIntervalSeconds)
    , Velocity(InVelocity)
{
}

void EnemyBase::Tick(float DeltaTime)
{
    CharacterActor::Tick(DeltaTime);
    Move(DeltaTime);
}

void EnemyBase::Move(float DeltaTime)
{
    SetPosition(GetPosition() + Velocity * DeltaTime);
}
