#include "BackgroundActor.h"

#include <SDL3/SDL.h>

#include "../Base/Types.h"
#include "../Resource/ResourcePaths.h"

BackgroundActor::BackgroundActor()
    : Actor(
        Vector2D(0.0f, 0.0f),
        0.0f,
        1,
        Vector2D(static_cast<float>(CLUCK_WINDOW_WIDTH), static_cast<float>(CLUCK_WINDOW_HEIGHT)),
        1.0f)
{
}

void BackgroundActor::Render(SDL_Renderer* Renderer)
{
    if (!EnsureTextureLoaded(Renderer))
    {
        return;
    }

    const SDL_FRect& BackgroundRect = GetRenderRect();
    SDL_RenderTexture(Renderer, BackgroundTexture.get(), nullptr, &BackgroundRect);
}

bool BackgroundActor::EnsureTextureLoaded(SDL_Renderer* Renderer)
{
    if (Renderer == nullptr)
    {
        return false;
    }

    if (BackgroundTexture)
    {
        return true;
    }

    SDL_Surface* BackgroundSurface = SDL_LoadPNG(Resources::Image::Background);
    if (BackgroundSurface == nullptr)
    {
        return false;
    }

    BackgroundTexture.reset(SDL_CreateTextureFromSurface(Renderer, BackgroundSurface));
    SDL_DestroySurface(BackgroundSurface);
    return BackgroundTexture != nullptr;
}
