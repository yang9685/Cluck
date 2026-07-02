#include "Actor.h"

void Actor::BeginPlay()
{
    bHasBegunPlay = true;
}

void Actor::Tick(float DeltaTime)
{
    (void)DeltaTime;
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
