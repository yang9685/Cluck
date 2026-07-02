#pragma once
#include <memory>
#include "../Base/Atlas.h"

struct AnimationPlaybackState
{
    bool bLoop = false;
    bool bPlaying = false;
    bool bPaused = false;
    float PlayRate = 1.0f;
};

class AnimationComponent
{
public:
    AnimationComponent() = default;
    explicit AnimationComponent(std::unique_ptr<Atlas> InAtlas);

    void SetAtlas(std::unique_ptr<Atlas> InAtlas);
    bool HasAtlas() const;
    Atlas& GetAtlas();
    const Atlas& GetAtlas() const;

    void Start(bool bLoop = false, float PlayRate = 1.0f);
    void Stop();
    void Pause();
    void Resume();

    const AnimationPlaybackState& GetPlaybackState() const;
    bool IsPlaying() const;
    bool IsPaused() const;

private:
    std::unique_ptr<Atlas> AtlasAsset;
    AnimationPlaybackState PlaybackState;
};
