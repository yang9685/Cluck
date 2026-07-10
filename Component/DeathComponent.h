#pragma once

#include "AnimationComponent.h"
#include "Component.h"

class Actor;

class DeathComponent : public Component
{
public:
    DeathComponent(Actor& InOwner, AnimationComponent& InAnimationComponent);

    void OnDeathBegin();
    bool IsDying() const;

private:
    Actor& Owner;
    AnimationComponent& Animation;
    bool bDying = false;
};
