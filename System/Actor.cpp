#include "Actor.h"

#include <SDL3/SDL.h>
#include <stdexcept>

Actor::Actor(const Vector2D& InPosition, float InRotation, int InRenderPriority, const Vector2D& InBaseRenderSize, float InScale)
{
    Position = InPosition;
    Rotation = InRotation;
    RenderPriority = InRenderPriority;
    BaseRenderSize = InBaseRenderSize;
    Scale = InScale;
    UpdateRenderRect();
}

void Actor::BeginPlay()
{
    if (bHasBegunPlay)
    {
        return;
    }
    
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

void Actor::Render(SDL_Renderer* Renderer)
{
    (void)Renderer;
}

TexturePtr Actor::LoadTexture(SDL_Renderer* Renderer, const char* TexturePath)
{
    if (Renderer == nullptr)
    {
        return TexturePtr(nullptr);
    }

    SDL_Surface* TextureSurface = SDL_LoadPNG(TexturePath);
    if (TextureSurface == nullptr)
    {
        return TexturePtr(nullptr);
    }

    TexturePtr Texture(SDL_CreateTextureFromSurface(Renderer, TextureSurface));
    SDL_DestroySurface(TextureSurface);
    return Texture;
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
    UpdateRenderRect();
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

const SDL_FRect& Actor::GetRenderRect() const
{
    return RenderRect;
}

void Actor::SetBaseRenderSize(const Vector2D& InBaseRenderSize)
{
    BaseRenderSize = InBaseRenderSize;
    UpdateRenderRect();
}

const Vector2D& Actor::GetBaseRenderSize() const
{
    return BaseRenderSize;
}

void Actor::SetScale(float InScale)
{
    Scale = InScale;
    UpdateRenderRect();
}

float Actor::GetScale() const
{
    return Scale;
}

int Actor::GetRenderPriority() const
{
    return RenderPriority;
}

void Actor::UpdateRenderRect()
{
    RenderRect.x = Position.x;
    RenderRect.y = Position.y;
    RenderRect.w = BaseRenderSize.x * Scale;
    RenderRect.h = BaseRenderSize.y * Scale;
}
