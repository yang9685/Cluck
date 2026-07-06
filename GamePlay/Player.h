#pragma once

#include "CharacterActor.h"

class Player : public CharacterActor
{
public:
    Player(const Vector2D& InPosition, float InRotation);

    void Render(SDL_Renderer* Renderer) override;
    void Fire();

private:
    TexturePtr BarrelIdleTexture;
    TexturePtr BatteryTexture;
    SDL_FRect BarrelRenderRect = {0.0f, 0.0f, 0.0f, 0.0f};
    SDL_FRect BatteryRenderRect = {0.0f, 0.0f, 0.0f, 0.0f};
    bool bFire = false;
};
