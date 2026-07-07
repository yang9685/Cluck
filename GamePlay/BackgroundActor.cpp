#include "BackgroundActor.h"

#include <algorithm>

#include <SDL3/SDL.h>

#include "../Base/Types.h"
#include "../Resource/ResourcePaths.h"

namespace
{
constexpr float BackgroundSourceWidth = 1344.0f;
constexpr float BackgroundSourceHeight = 800.0f;

float GetBackgroundScale()
{
    const float WidthScale = static_cast<float>(CLUCK_WINDOW_WIDTH) / BackgroundSourceWidth;
    const float HeightScale = static_cast<float>(CLUCK_WINDOW_HEIGHT) / BackgroundSourceHeight;
    return std::max(WidthScale, HeightScale);
}

Vector2D GetBackgroundPosition()
{
    const float BackgroundScale = GetBackgroundScale();
    const float BackgroundWidth = BackgroundSourceWidth * BackgroundScale;
    const float BackgroundHeight = BackgroundSourceHeight * BackgroundScale;
    return Vector2D(
        (static_cast<float>(CLUCK_WINDOW_WIDTH) - BackgroundWidth) * 0.5f,
        (static_cast<float>(CLUCK_WINDOW_HEIGHT) - BackgroundHeight) * 0.5f);
}

Vector2D GetBackgroundBaseRenderSize()
{
    const float BackgroundScale = GetBackgroundScale();
    return Vector2D(
        BackgroundSourceWidth * BackgroundScale,
        BackgroundSourceHeight * BackgroundScale);
}
}

BackgroundActor::BackgroundActor()
    : Actor(
        GetBackgroundPosition(),
        0.0f,
        1,
        GetBackgroundBaseRenderSize(),
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
    if (BackgroundTexture)
    {
        return true;
    }

    BackgroundTexture = LoadTexture(Renderer, Resources::Image::Background);
    return BackgroundTexture != nullptr;
}
