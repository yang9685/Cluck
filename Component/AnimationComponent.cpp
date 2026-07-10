#include "AnimationComponent.h"

#include <stdexcept>

#include <SDL3/SDL_render.h>

#include "../System/RenderManager.h"

AnimationComponent::AnimationComponent(std::unique_ptr<Atlas> InAtlas, float InFrameIntervalSeconds)
    : AtlasAsset(std::move(InAtlas))
    , FrameIntervalSeconds(InFrameIntervalSeconds)
{
}

AnimationComponent::~AnimationComponent()
{
    DestroyTimer();
}

void AnimationComponent::BeginPlay()
{
    Component::BeginPlay();
}

void AnimationComponent::SetAtlas(std::unique_ptr<Atlas> InAtlas)
{
    AtlasAsset = std::move(InAtlas);
}

bool AnimationComponent::HasAtlas() const
{
    return static_cast<bool>(AtlasAsset);
}

Atlas& AnimationComponent::GetAtlas()
{
    if (!AtlasAsset)
    {
        throw std::logic_error("Animation component atlas is not set");
    }

    return *AtlasAsset;
}

const Atlas& AnimationComponent::GetAtlas() const
{
    if (!AtlasAsset)
    {
        throw std::logic_error("Animation component atlas is not set");
    }

    return *AtlasAsset;
}

bool AnimationComponent::Render(
    SDL_Renderer* Renderer,
    const SDL_FRect& DestRect,
    double AngleDegrees,
    const SDL_FPoint& RotationCenter) const
{
    if (Renderer == nullptr || !AtlasAsset)
    {
        return false;
    }

    SDL_Texture* CurrentFrameTexture = AtlasAsset->GetCurrentFrameTexture();
    if (CurrentFrameTexture == nullptr)
    {
        return false;
    }

    return RenderManager::GetInstance().RenderTextureRotated(
        CurrentFrameTexture,
        DestRect,
        AngleDegrees,
        RotationCenter);
}

void AnimationComponent::Start(bool bLoop, float PlayRate, std::function<void()> OnFinished)
{
    DestroyTimer();

    PlaybackState.bLoop = bLoop;
    PlaybackState.bPlaying = true;
    PlaybackState.bPaused = false;
    PlaybackState.PlayRate = PlayRate;
    FinishCallBack = std::move(OnFinished);

    if (AtlasAsset)
    {
        AtlasAsset->Reset();
    }

    if (HasBegunPlay())
    {
        CreateTimer();
    }
}

void AnimationComponent::Stop()
{
    PlaybackState.bPlaying = false;
    PlaybackState.bPaused = false;

    if (AtlasAsset)
    {
        AtlasAsset->Reset();
    }

    FinishCallBack = {};
    DestroyTimer();
}

void AnimationComponent::Pause()
{
    if (!PlaybackState.bPlaying)
    {
        return;
    }

    PlaybackState.bPaused = true;
    if (FrameAdvanceTimerHandle != InvalidTimerHandle)
    {
        TimeManager.get().StopTimer(FrameAdvanceTimerHandle);
    }
}

void AnimationComponent::Resume()
{
    if (!PlaybackState.bPlaying)
    {
        return;
    }

    PlaybackState.bPaused = false;
    if (FrameAdvanceTimerHandle != InvalidTimerHandle)
    {
        TimeManager.get().StartTimer(FrameAdvanceTimerHandle);
    }
}

void AnimationComponent::SetFrameIntervalSeconds(float InFrameIntervalSeconds)
{
    FrameIntervalSeconds = InFrameIntervalSeconds;

    if (HasBegunPlay() && PlaybackState.bPlaying)
    {
        CreateTimer();
        if (PlaybackState.bPaused && FrameAdvanceTimerHandle != InvalidTimerHandle)
        {
            TimeManager.get().StopTimer(FrameAdvanceTimerHandle);
        }
    }
}

float AnimationComponent::GetFrameIntervalSeconds() const
{
    return FrameIntervalSeconds;
}

const AnimationPlaybackState& AnimationComponent::GetPlaybackState() const
{
    return PlaybackState;
}

bool AnimationComponent::IsPlaying() const
{
    return PlaybackState.bPlaying;
}

bool AnimationComponent::IsPaused() const
{
    return PlaybackState.bPaused;
}

void AnimationComponent::AdvanceAnimation()
{
    if (!PlaybackState.bPlaying || PlaybackState.bPaused || !AtlasAsset)
    {
        return;
    }

    const bool bReachedAnimationEnd = AtlasAsset->IsLastFrame();
    AtlasAsset->AdvanceFrame(PlaybackState.bLoop);
    HandleAnimationProgress();

    if (!bReachedAnimationEnd || !FinishCallBack)
    {
        return;
    }

    std::function<void()> CurrentFinishCallBack = std::move(FinishCallBack);
    CurrentFinishCallBack();
}

void AnimationComponent::HandleAnimationProgress()
{
    if (!AtlasAsset || !AtlasAsset->IsFinished())
    {
        return;
    }

    if (!PlaybackState.bLoop)
    {
        PlaybackState.bPlaying = false;
        PlaybackState.bPaused = false;
        DestroyTimer();
        return;
    }

    AtlasAsset->Reset();
}

void AnimationComponent::CreateTimer()
{
    DestroyTimer();

    if (FrameIntervalSeconds <= 0.0f)
    {
        return;
    }

    FrameAdvanceTimerHandle = TimeManager.get().CreateTimer(
        GetEffectiveFrameIntervalSeconds(),
        true,
        [this]()
        {
            AdvanceAnimation();
        });
}

void AnimationComponent::DestroyTimer()
{
    if (FrameAdvanceTimerHandle != InvalidTimerHandle)
    {
        TimeManager.get().DestroyTimer(FrameAdvanceTimerHandle);
        FrameAdvanceTimerHandle = InvalidTimerHandle;
    }
}

float AnimationComponent::GetEffectiveFrameIntervalSeconds() const
{
    if (PlaybackState.PlayRate <= 0.0f)
    {
        return FrameIntervalSeconds;
    }

    return FrameIntervalSeconds / PlaybackState.PlayRate;
}
