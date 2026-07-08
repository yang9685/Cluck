#pragma once

#include "../System/Actor.h"

class Bullet : public Actor
{
public:
    Bullet(
        const Vector2D& InPosition,
        float InRotation,
        float InSpeed = 800.0f,
        float InScale = 1.0f);

    void Tick(float DeltaTime) override;
    void Render(SDL_Renderer* Renderer) override;
    void Destroy();

    void SetSpeed(float InSpeed);
    float GetSpeed() const;

protected:
    void Move(float DeltaTime);

private:
    bool IsOutOfScreen() const;

    TexturePtr BulletTexture;
    float Speed = 800.0f;
    SDL_FPoint Rotation_Center = { 0.0f, 0.0f };
};
