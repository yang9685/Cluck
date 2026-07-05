#include "AnimationComponent.h"

#include <stdexcept>

#include <SDL3/SDL_render.h>

AnimationComponent::AnimationComponent(std::unique_ptr<Atlas> InAtlas, float InFrameIntervalSeconds)
    : AtlasAsset(std::move(InAtlas))
    , FrameIntervalSeconds(InFrameIntervalSeconds)
{
}

AnimationComponent::~AnimationComponent()
{
    if (FrameAdvanceTimerHandle != InvalidTimerHandle)
    {
        TimeManager.get().DestroyTimer(FrameAdvanceTimerHandle);
        FrameAdvanceTimerHandle = InvalidTimerHandle;
    }
}

void AnimationComponent::BeginPlay()
{
    Component::BeginPlay();
    RecreateTimer();
    SyncTimerState();
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

bool AnimationComponent::Render(SDL_Renderer* Renderer, const SDL_FRect& DestRect) const
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

    SDL_RenderTexture(Renderer, CurrentFrameTexture, nullptr, &DestRect);
    return true;
}

void AnimationComponent::Start(bool bLoop, float PlayRate)
{
    PlaybackState.bLoop = bLoop;
    PlaybackState.bPlaying = true;
    PlaybackState.bPaused = false;
    PlaybackState.PlayRate = PlayRate;

    if (AtlasAsset)
    {
        AtlasAsset->Reset();
    }

    if (HasBegunPlay())
    {
        RecreateTimer();
        SyncTimerState();
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

    SyncTimerState();
}

void AnimationComponent::Pause()
{
    if (!PlaybackState.bPlaying)
    {
        return;
    }

    PlaybackState.bPaused = true;
    SyncTimerState();
}

void AnimationComponent::Resume()
{
    if (!PlaybackState.bPlaying)
    {
        return;
    }

    PlaybackState.bPaused = false;
    SyncTimerState();
}

void AnimationComponent::SetFrameIntervalSeconds(float InFrameIntervalSeconds)
{
    FrameIntervalSeconds = InFrameIntervalSeconds;

    if (HasBegunPlay())
    {
        RecreateTimer();
        SyncTimerState();
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

    const bool bAdvanced = AtlasAsset->AdvanceFrame(PlaybackState.bLoop);
    if (bAdvanced)
    {
        return;
    }

    if (!PlaybackState.bLoop && AtlasAsset->IsFinished())
    {
        PlaybackState.bPlaying = false;
        PlaybackState.bPaused = false;
        SyncTimerState();
    }
}

void AnimationComponent::RecreateTimer()
{
    if (!HasBegunPlay())
    {
        return;
    }

    if (FrameAdvanceTimerHandle != InvalidTimerHandle)
    {
        TimeManager.get().DestroyTimer(FrameAdvanceTimerHandle);
        FrameAdvanceTimerHandle = InvalidTimerHandle;
    }

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

void AnimationComponent::SyncTimerState()
{
    if (FrameAdvanceTimerHandle == InvalidTimerHandle)
    {
        return;
    }

    if (PlaybackState.bPlaying && !PlaybackState.bPaused)
    {
        TimeManager.get().StartTimer(FrameAdvanceTimerHandle);
        return;
    }

    TimeManager.get().StopTimer(FrameAdvanceTimerHandle);
}

float AnimationComponent::GetEffectiveFrameIntervalSeconds() const
{
    if (PlaybackState.PlayRate <= 0.0f)
    {
        return FrameIntervalSeconds;
    }

    return FrameIntervalSeconds / PlaybackState.PlayRate;
}
