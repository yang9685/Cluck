#include "RenderManager.h"

#include <algorithm>
#include <SDL3/SDL_render.h>

#include "CameraManager.h"

void RenderManager::Initialize(SDL_Renderer* InRenderer)
{
    Renderer = InRenderer;
}

void RenderManager::Update(const std::vector<std::unique_ptr<Actor>>& Actors)
{
    if (Renderer == nullptr)
    {
        return;
    }

    SDL_RenderClear(Renderer);
    for (const auto& Actor : Actors)
    {
        Actor->Render(Renderer);
    }
    SDL_RenderPresent(Renderer);

}

SDL_Renderer* RenderManager::GetRenderer() const
{
    return Renderer;
}

SDL_FRect RenderManager::ApplyCameraOffset(const SDL_FRect& InRect) const
{
    SDL_FRect OffsetRect = InRect;
    const Vector2D& CameraOffset = CameraManager::GetInstance().GetCurrentOffset();
    OffsetRect.x += CameraOffset.x;
    OffsetRect.y += CameraOffset.y;
    return OffsetRect;
}

bool RenderManager::RenderTexture(SDL_Texture* Texture, const SDL_FRect& DestRect, const SDL_FRect* SourceRect) const
{
    if (Renderer == nullptr || Texture == nullptr)
    {
        return false;
    }

    const SDL_FRect FinalRect = ApplyCameraOffset(DestRect);
    return SDL_RenderTexture(Renderer, Texture, SourceRect, &FinalRect);
}

bool RenderManager::RenderTextureRotated(
    SDL_Texture* Texture,
    const SDL_FRect& DestRect,
    double AngleDegrees,
    const SDL_FPoint& RotationCenter,
    const SDL_FRect* SourceRect,
    SDL_FlipMode Flip) const
{
    if (Renderer == nullptr || Texture == nullptr)
    {
        return false;
    }

    const SDL_FRect FinalRect = ApplyCameraOffset(DestRect);
    return SDL_RenderTextureRotated(
        Renderer,
        Texture,
        SourceRect,
        &FinalRect,
        AngleDegrees,
        &RotationCenter,
        Flip);
}
