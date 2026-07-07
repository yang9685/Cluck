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

void EnemyBase::BeginPlay()
{
    CharacterActor::BeginPlay();
    GetAnimationComponent().Start(true);
}

void EnemyBase::Tick(float DeltaTime)
{
    CharacterActor::Tick(DeltaTime);
    Move(DeltaTime);
}

void EnemyBase::Render(SDL_Renderer* Renderer)
{
    const SDL_FRect& RenderRect = GetRenderRect();
    const SDL_FPoint RotationCenter = {
        RenderRect.w * 0.5f,
        RenderRect.h * 0.5f,
    };
    GetAnimationComponent().Render(Renderer, RenderRect, GetRotation(), RotationCenter);
}

void EnemyBase::Move(float DeltaTime)
{
    SetPosition(GetPosition() + Velocity * DeltaTime);
}
