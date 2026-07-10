#pragma once

#include "../Base/Vector2D.h"

class CameraManager
{
private:
    CameraManager() = default;
    ~CameraManager() = default;

    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

public:
    static CameraManager& GetInstance()
    {
        static CameraManager Instance;
        return Instance;
    }

    void Initialize();
    void Update(float DeltaTime);
    void StartRecoilShake(float AngleDegrees, float DurationSeconds, float Amplitude);
    const Vector2D& GetCurrentOffset() const;

private:
    Vector2D CurrentOffset = Vector2D(0.0f, 0.0f);
    Vector2D RecoilDirection = Vector2D(0.0f, 0.0f);
    float ShakeTimeRemaining = 0.0f;
    float ShakeDuration = 0.0f;
    float ShakeAmplitude = 0.0f;
};
