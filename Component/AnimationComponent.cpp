#include "AnimationComponent.h"
#include <stdexcept>

AnimationComponent::AnimationComponent(std::unique_ptr<Atlas> InAtlas)
    : AtlasAsset(std::move(InAtlas))
{
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

void AnimationComponent::Start(bool bLoop, float PlayRate)
{
    PlaybackState.bLoop = bLoop;
    PlaybackState.bPlaying = true;
    PlaybackState.bPaused = false;
    PlaybackState.PlayRate = PlayRate;
}

void AnimationComponent::Stop()
{
    PlaybackState.bPlaying = false;
    PlaybackState.bPaused = false;
}

void AnimationComponent::Pause()
{
    if (PlaybackState.bPlaying)
    {
        PlaybackState.bPaused = true;
    }
}

void AnimationComponent::Resume()
{
    if (PlaybackState.bPlaying)
    {
        PlaybackState.bPaused = false;
    }
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
