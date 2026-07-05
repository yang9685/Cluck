#include "CharacterActor.h"

CharacterActor::CharacterActor(
    std::unique_ptr<Atlas> InAtlas,
    const Vector2D& InPosition,
    float InRotation,
    int InRenderPriority,
    const Vector2D& InBaseRenderSize,
    float InScale,
    float InFrameIntervalSeconds)
    : Actor(InPosition, InRotation, InRenderPriority, InBaseRenderSize, InScale)
    , Animation(CreateAnimationComponent(*this, std::move(InAtlas), InFrameIntervalSeconds))
{
}

void CharacterActor::Render(SDL_Renderer* Renderer)
{
    Animation.Render(Renderer, GetRenderRect());
}

AnimationComponent& CharacterActor::GetAnimationComponent()
{
    return Animation;
}

const AnimationComponent& CharacterActor::GetAnimationComponent() const
{
    return Animation;
}

AnimationComponent& CharacterActor::CreateAnimationComponent(
    CharacterActor& Owner,
    std::unique_ptr<Atlas> InAtlas,
    float InFrameIntervalSeconds)
{
    std::unique_ptr<AnimationComponent> NewAnimationComponent =
        std::make_unique<AnimationComponent>(std::move(InAtlas), InFrameIntervalSeconds);
    Component& NewComponent = Owner.AddComponent(std::move(NewAnimationComponent));
    return static_cast<AnimationComponent&>(NewComponent);
}
