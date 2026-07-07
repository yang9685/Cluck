#include "Player.h"

#include <algorithm>
#include <cmath>

#include "../Base/Types.h"
#include "../Resource/ResourcePaths.h"

namespace
{
constexpr float BackgroundSourceWidth = 1344.0f;
constexpr float BackgroundSourceHeight = 800.0f;
constexpr float PlayerAnchorBackgroundX = 672.0f;
constexpr float PlayerAnchorBackgroundY = 642.0f;

float GetBackgroundScale()
{
    const float WidthScale = static_cast<float>(CLUCK_WINDOW_WIDTH) / BackgroundSourceWidth;
    const float HeightScale = static_cast<float>(CLUCK_WINDOW_HEIGHT) / BackgroundSourceHeight;
    return std::max(WidthScale, HeightScale);
}

Vector2D GetPlayerAnchorOnScreen()
{
    const float BackgroundScale = GetBackgroundScale();
    const float BackgroundWidth = BackgroundSourceWidth * BackgroundScale;
    const float BackgroundHeight = BackgroundSourceHeight * BackgroundScale;
    const float BackgroundX = (static_cast<float>(CLUCK_WINDOW_WIDTH) - BackgroundWidth) * 0.5f;
    const float BackgroundY = (static_cast<float>(CLUCK_WINDOW_HEIGHT) - BackgroundHeight) * 0.5f;
    return Vector2D(
        BackgroundX + PlayerAnchorBackgroundX * BackgroundScale,
        BackgroundY + PlayerAnchorBackgroundY * BackgroundScale);
}
}

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
    const Vector2D PlayerAnchorOnScreen = GetPlayerAnchorOnScreen();

    BarrelRenderRect = {
        PlayerAnchorOnScreen.x - 250.0f * Scale * 0.23f,
        PlayerAnchorOnScreen.y - 50.0f * Scale * 0.5f,
        250.0f * Scale,
        50.0f * Scale};
    BatteryRenderRect = {
        PlayerAnchorOnScreen.x - 125.0f * Scale * 0.5f,
        PlayerAnchorOnScreen.y - 73.0f * Scale,
        125.0f * Scale,
        125.0f * Scale};

    BarrelRotateCenter = {BarrelRenderRect.w * 0.23f, BarrelRenderRect.h * 0.5f};
    BatteryRotateCenter = {BatteryRenderRect.w, BatteryRenderRect.h};
}

void Player::Tick(float DeltaTime)
{
    CharacterActor::Tick(DeltaTime);

    const float CurrentRotation = GetRotation();
    const float RotationDelta = NormalizeAngleDegrees(TargetRotation - CurrentRotation);
    const float InterpAlpha = std::min(1.0f, DeltaTime * RotationInterpSpeed);
    SetRotation(CurrentRotation + RotationDelta * InterpAlpha);
}

void Player::Render(SDL_Renderer* Renderer)
{
    const double RotationAngle = GetRotation();

    if (BatteryTexture)
    {
        SDL_RenderTextureRotated(
            Renderer,
            BatteryTexture.get(),
            nullptr,
            &BatteryRenderRect,
            0,
            &BatteryRotateCenter,
            SDL_FLIP_NONE);
    }

    if (!bFire)
    {
        if (BarrelIdleTexture)
        {
            SDL_RenderTextureRotated(
                Renderer,
                BarrelIdleTexture.get(),
                nullptr,
                &BarrelRenderRect,
                RotationAngle,
                &BarrelRotateCenter,
                SDL_FLIP_NONE);
        }

        return;
    }

    GetAnimationComponent().Render(Renderer, BarrelRenderRect, RotationAngle, BarrelRotateCenter);
}

void Player::Fire()
{
    if (bFire)
    {
        return;
    }
    bFire = true;
    GetAnimationComponent().Start(false, 2.0f, [this]()
    {
        bFire = false;
    });
}

void Player::SetTargetRotation(float InTargetRotation)
{
    TargetRotation = InTargetRotation;
}

float Player::GetTargetRotation() const
{
    return TargetRotation;
}
