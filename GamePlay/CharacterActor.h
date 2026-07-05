#pragma once

#include <array>
#include <utility>

#include <SDL3/SDL.h>

#include "../Base/Atlas.h"
#include "../Component/AnimationComponent.h"
#include "../System/RenderManager.h"
#include "../System/Actor.h"

class CharacterActor : public Actor
{
public:
    CharacterActor(
        std::unique_ptr<Atlas> InAtlas,
        const Vector2D& InPosition,
        float InRotation,
        int InRenderPriority,
        const Vector2D& InBaseRenderSize,
        float InScale,
        float InFrameIntervalSeconds = 0.1f);

    void Render(SDL_Renderer* Renderer) override;

    AnimationComponent& GetAnimationComponent();
    const AnimationComponent& GetAnimationComponent() const;

protected:
    template <size_t N>
    static std::unique_ptr<Atlas> CreateAtlasFromPaths(
        SDL_Renderer* Renderer,
        const std::array<const char*, N>& TexturePaths)
    {
        if (Renderer == nullptr)
        {
            return nullptr;
        }

        std::unique_ptr<Atlas> NewAtlas = std::make_unique<Atlas>();
        for (const char* TexturePath : TexturePaths)
        {
            SDL_Surface* TextureSurface = SDL_LoadPNG(TexturePath);
            if (TextureSurface == nullptr)
            {
                continue;
            }

            TexturePtr Texture(SDL_CreateTextureFromSurface(Renderer, TextureSurface));
            SDL_DestroySurface(TextureSurface);

            if (Texture)
            {
                NewAtlas->AddFrame(std::move(Texture));
            }
        }

        if (!NewAtlas->IsValid())
        {
            return nullptr;
        }

        return NewAtlas;
    }

private:
    static AnimationComponent& CreateAnimationComponent(
        CharacterActor& Owner,
        std::unique_ptr<Atlas> InAtlas,
        float InFrameIntervalSeconds);

private:
    AnimationComponent& Animation;
};
