#pragma once
#include "../Base/Vector2D.h"

class Actor
{
public:
    Actor() = default;
    virtual ~Actor() = default;

    virtual void BeginPlay();
    virtual void Tick(float DeltaTime);

    void SetPosition(const Vector2D& InPosition);
    const Vector2D& GetPosition() const;
    void SetRotation(float InRotation);
    float GetRotation() const;
    bool HasBegunPlay() const;

protected:
    Vector2D Position;
    float Rotation = 0.0f;
    bool bHasBegunPlay = false;
};
