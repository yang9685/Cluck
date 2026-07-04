#include "Actor.h"

#include <stdexcept>

Actor::Actor(const Vector2D position, const float rotation,const int RenderPriority)
{
    this->Position = position;
    this->Rotation = rotation;
    this->RenderPriority = RenderPriority;
}

void Actor::BeginPlay()
{
    for (const std::unique_ptr<Component>& ComponentInstance : Components)
    {
        if (ComponentInstance)
        {
            ComponentInstance->BeginPlay();
        }
    }

    bHasBegunPlay = true;
}

void Actor::Tick(float DeltaTime)
{
    (void)DeltaTime;
}

void Actor::Render(SDL_Renderer* renderer)
{
}

Component& Actor::AddComponent(std::unique_ptr<Component> NewComponent)
{
    if (!NewComponent)
    {
        throw std::invalid_argument("Cannot add a null component");
    }

    Component& ComponentRef = *NewComponent;
    Components.push_back(std::move(NewComponent));

    if (bHasBegunPlay)
    {
        ComponentRef.BeginPlay();
    }

    return ComponentRef;
}

void Actor::SetPosition(const Vector2D& InPosition)
{
    Position = InPosition;
}

const Vector2D& Actor::GetPosition() const
{
    return Position;
}

void Actor::SetRotation(float InRotation)
{
    Rotation = InRotation;
}

float Actor::GetRotation() const
{
    return Rotation;
}

bool Actor::HasBegunPlay() const
{
    return bHasBegunPlay;
}

const std::vector<std::unique_ptr<Component>>& Actor::GetComponents() const
{
    return Components;
}
