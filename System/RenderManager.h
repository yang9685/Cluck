#pragma once
#include <memory>
#include <vector>

#include <SDL3/SDL_render.h>

#include "Actor.h"

class RenderManager
{
private:
    RenderManager() = default;
    ~RenderManager() = default;

    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;

public:
    static RenderManager& GetInstance()
    {
        static RenderManager Instance;
        return Instance;
    }

    void Initialize(SDL_Renderer* Renderer);
    void Update(const std::vector<std::unique_ptr<Actor>>& Actors);
    SDL_Renderer* GetRenderer() const;
    SDL_FRect ApplyCameraOffset(const SDL_FRect& InRect) const;
    bool RenderTexture(SDL_Texture* Texture, const SDL_FRect& DestRect, const SDL_FRect* SourceRect = nullptr) const;
    bool RenderTextureRotated(
        SDL_Texture* Texture,
        const SDL_FRect& DestRect,
        double AngleDegrees,
        const SDL_FPoint& RotationCenter,
        const SDL_FRect* SourceRect = nullptr,
        SDL_FlipMode Flip = SDL_FLIP_NONE) const;

private:
    SDL_Renderer* Renderer = nullptr;
};
