#include "Bullet.h"

#include <cmath>

#include <SDL3/SDL.h>

#include "../Base/Types.h"
#include "../Resource/ResourcePaths.h"
#include "../System/GameActivityBase.h"
#include "../System/RenderManager.h"

namespace
{
constexpr float DegreesToRadians = 3.14159265358979323846f / 180.0f;
}

Bullet::Bullet(const Vector2D& InPosition, float InRotation, float InSpeed, float InScale)
    : Actor(
        InPosition,
        InRotation,
        25,
        Vector2D(8.0f, 3.0f),
        InScale)
    , Speed(InSpeed)
{
    BulletTexture = LoadTexture(RenderManager::GetInstance().GetRenderer(), Resources::Image::Bullet);
    Rotation_Center = {
        GetRenderRect().w * 0.5f,
        GetRenderRect().h * 0.5f,
    };
    
}

void Bullet::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);
    Move(DeltaTime);

    if (IsOutOfScreen())
    {
        Destroy();
    }
}

void Bullet::Render(SDL_Renderer* Renderer)
{
    if (BulletTexture == nullptr)
    {
        return;
    }

    RenderManager::GetInstance().RenderTextureRotated(
        BulletTexture.get(),
        GetRenderRect(),
        GetRotation(),
        Rotation_Center);
}

void Bullet::SetSpeed(float InSpeed)
{
    Speed = InSpeed;
}

void Bullet::Destroy()
{
    GameActivityBase::GetInstance().DestroyActor(*this);
}

float Bullet::GetSpeed() const
{
    return Speed;
}

void Bullet::Move(float DeltaTime)
{
    const float RotationRadians = GetRotation() * DegreesToRadians;
    const Vector2D ForwardVector(std::cos(RotationRadians), std::sin(RotationRadians));
    SetPosition(GetPosition() + ForwardVector * Speed * DeltaTime);
}

bool Bullet::IsOutOfScreen() const
{
    const SDL_FRect& BulletRenderRect = GetRenderRect();
    return BulletRenderRect.x + BulletRenderRect.w < 0.0f ||
        BulletRenderRect.x > static_cast<float>(CLUCK_WINDOW_WIDTH) ||
        BulletRenderRect.y + BulletRenderRect.h < 0.0f ||
        BulletRenderRect.y > static_cast<float>(CLUCK_WINDOW_HEIGHT);
}
