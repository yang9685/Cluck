#pragma once

#include <memory>
#include <vector>
#include <SDL3/SDL_render.h>

#include "../Component/Component.h"
#include "../Base/Vector2D.h"

class Actor
{
public:
    Actor() = default;
    Actor(const Vector2D position, const float rotation, const int RenderPriority);
    virtual ~Actor() = default;

    virtual void BeginPlay();
    virtual void Tick(float DeltaTime);
    virtual void Render(SDL_Renderer* renderer);
    Component& AddComponent(std::unique_ptr<Component> NewComponent);


    
    void SetPosition(const Vector2D& InPosition);
    const Vector2D& GetPosition() const;
    void SetRotation(float InRotation);
    float GetRotation() const;
    bool HasBegunPlay() const;
    const std::vector<std::unique_ptr<Component>>& GetComponents() const;

    
    int RenderPriority;
    std::vector<std::unique_ptr<Component>> Components;
    Vector2D Position;
    float Rotation = 0.0f;
    bool bHasBegunPlay = false;
};
