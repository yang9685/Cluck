#pragma once

#include "CharacterActor.h"

class Player : public CharacterActor
{
public:
    Player(const Vector2D& InPosition, float InRotation);

    void Tick(float DeltaTime) override;
    void Render(SDL_Renderer* Renderer) override;
    void Fire();
    void SetTargetRotation(float InTargetRotation);
    float GetTargetRotation() const;

private:
    TexturePtr BarrelIdleTexture;
    TexturePtr BatteryTexture;
    SDL_FRect BarrelRenderRect = {0.0f, 0.0f, 0.0f, 0.0f};
    SDL_FRect BatteryRenderRect = {0.0f, 0.0f, 0.0f, 0.0f};
    SDL_FPoint BarrelRotateCenter = {0.0f, 0.0f};
    SDL_FPoint BatteryRotateCenter = {0.0f, 0.0f};
    
    bool bFire = false;
    float TargetRotation = 0.0f;
    float RotationInterpSpeed = 12.0f;
};
