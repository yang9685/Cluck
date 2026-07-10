#include "CameraManager.h"

#include <algorithm>
#include <cmath>

namespace
{
constexpr float DegreesToRadians = 3.14159265358979323846f / 180.0f;
}

void CameraManager::Initialize()
{
    CurrentOffset = Vector2D(0.0f, 0.0f);
    RecoilDirection = Vector2D(0.0f, 0.0f);
    ShakeTimeRemaining = 0.0f;
    ShakeDuration = 0.0f;
    ShakeAmplitude = 0.0f;
}

void CameraManager::Update(float DeltaTime)
{
    if (ShakeTimeRemaining <= 0.0f || ShakeDuration <= 0.0f || ShakeAmplitude <= 0.0f)
    {
        CurrentOffset = Vector2D(0.0f, 0.0f);
        RecoilDirection = Vector2D(0.0f, 0.0f);
        ShakeTimeRemaining = 0.0f;
        return;
    }

    ShakeTimeRemaining = std::max(0.0f, ShakeTimeRemaining - DeltaTime);
    const float ShakeAlpha = ShakeTimeRemaining / ShakeDuration;
    const float CurrentAmplitude = ShakeAmplitude * ShakeAlpha;
    CurrentOffset = RecoilDirection * CurrentAmplitude;

    if (ShakeTimeRemaining <= 0.0f)
    {
        CurrentOffset = Vector2D(0.0f, 0.0f);
        RecoilDirection = Vector2D(0.0f, 0.0f);
    }
}

void CameraManager::StartRecoilShake(float AngleDegrees, float DurationSeconds, float Amplitude)
{
    const float RotationRadians = AngleDegrees * DegreesToRadians;
    RecoilDirection = Vector2D(-std::cos(RotationRadians), -std::sin(RotationRadians));
    ShakeDuration = DurationSeconds;
    ShakeTimeRemaining = DurationSeconds;
    ShakeAmplitude = Amplitude;
    CurrentOffset = RecoilDirection * Amplitude;
}

const Vector2D& CameraManager::GetCurrentOffset() const
{
    return CurrentOffset;
}
