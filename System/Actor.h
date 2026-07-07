#pragma once

#include <memory>
#include <vector>
#include <SDL3/SDL_render.h>

#include "../Base/Atlas.h"
#include "../Component/Component.h"
#include "../Base/Vector2D.h"

class Actor
{
public:
    Actor() = default;
    Actor(const Vector2D& Position, float Rotation, int RenderPriority, const Vector2D& BaseRenderSize, float Scale);
    virtual ~Actor() = default;

    virtual void BeginPlay();
    virtual void Tick(float DeltaTime);
    virtual void Render(SDL_Renderer* Renderer);
    Component& AddComponent(std::unique_ptr<Component> NewComponent);
    void SetPosition(const Vector2D& InPosition);
    const Vector2D& GetPosition() const;
    void SetRotation(float InRotation);
    float GetRotation() const;
    bool HasBegunPlay() const;
    const std::vector<std::unique_ptr<Component>>& GetComponents() const;
    const SDL_FRect& GetRenderRect() const;
    void SetBaseRenderSize(const Vector2D& InBaseRenderSize);
    const Vector2D& GetBaseRenderSize() const;
    void SetScale(float InScale);
    float GetScale() const;

protected:
    static TexturePtr LoadTexture(SDL_Renderer* Renderer, const char* TexturePath);
    static float NormalizeAngleDegrees(float AngleDegrees);
    int RenderPriority = 0;
    std::vector<std::unique_ptr<Component>> Components;
    Vector2D Position;
    Vector2D BaseRenderSize;
    SDL_FRect RenderRect = {0.0f, 0.0f, 0.0f, 0.0f};
    float Scale = 1.0f;
    float Rotation = 0.0f;
    bool bHasBegunPlay = false;

public:
    int GetRenderPriority() const;

private:
    void UpdateRenderRect();
};
