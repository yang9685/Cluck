#include "Player.h"

#include "../Resource/ResourcePaths.h"

Player::Player(const Vector2D& InPosition, float InRotation)
    : CharacterActor(
        CreateAtlasFromPaths(RenderManager::GetInstance().GetRenderer(), Resources::Image::BarrelFire),
        InPosition,
        InRotation,
        20,
        Vector2D(64.0f, 64.0f),
        1.5f,
        0.1f)
{
    SDL_Renderer* Renderer = RenderManager::GetInstance().GetRenderer();
    BarrelIdleTexture = LoadTexture(Renderer, Resources::Image::BarrelIdle);
    BatteryTexture = LoadTexture(Renderer, Resources::Image::Battery);

    BarrelRenderRect = {InPosition.x - 125*Scale/2,InPosition.y - 50*Scale/2,250 * Scale,50 * Scale};
    BatteryRenderRect = {InPosition.x- 125*Scale/2 ,InPosition.y - 125*Scale/2,125 * Scale,125 * Scale};
}

void Player::Render(SDL_Renderer* Renderer)
{
    if (BatteryTexture)
    {
        SDL_RenderTexture(Renderer, BatteryTexture.get(), nullptr, &BatteryRenderRect);
    }

    if (!bFire)
    {
        if (BarrelIdleTexture)
        {
            SDL_RenderTexture(Renderer, BarrelIdleTexture.get(), nullptr, &BarrelRenderRect);
        }

        return;
    }

    GetAnimationComponent().Render(Renderer, BarrelRenderRect);
}

void Player::Fire()
{
    bFire = true;
    GetAnimationComponent().Start(false, 1.0f, [this]()
    {
        bFire = false;
    });
}
