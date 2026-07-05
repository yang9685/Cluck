#pragma once

#include <functional>
#include <memory>

#include "../Base/Atlas.h"
#include "../System/TimerManager.h"
#include "Component.h"

struct AnimationPlaybackState
{
    bool bLoop = false;
    bool bPlaying = false;
    bool bPaused = false;
    float PlayRate = 1.0f;
};

class AnimationComponent : public Component
{
public:
    AnimationComponent() = default;
    explicit AnimationComponent(std::unique_ptr<Atlas> InAtlas, float InFrameIntervalSeconds = 0.1f);
    ~AnimationComponent() override;

    void BeginPlay() override;

    void SetAtlas(std::unique_ptr<Atlas> InAtlas);
    bool HasAtlas() const;
    Atlas& GetAtlas();
    const Atlas& GetAtlas() const;
    bool Render(SDL_Renderer* Renderer, const SDL_FRect& DestRect) const;

    void Start(bool bLoop = false, float PlayRate = 1.0f);
    void Stop();
    void Pause();
    void Resume();
    void SetFrameIntervalSeconds(float InFrameIntervalSeconds);
    float GetFrameIntervalSeconds() const;

    const AnimationPlaybackState& GetPlaybackState() const;
    bool IsPlaying() const;
    bool IsPaused() const;

private:
    void AdvanceAnimation();
    void RecreateTimer();
    void SyncTimerState();
    float GetEffectiveFrameIntervalSeconds() const;

private:
    std::unique_ptr<Atlas> AtlasAsset;
    AnimationPlaybackState PlaybackState;
    TimerHandle FrameAdvanceTimerHandle = InvalidTimerHandle;
    float FrameIntervalSeconds = 0.1f;
    std::reference_wrapper<TimerManager> TimeManager = TimerManager::GetInstance();
};
