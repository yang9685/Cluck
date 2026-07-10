#include "DeathComponent.h"

#include <array>

#include <SDL3/SDL.h>

#include "../Base/Atlas.h"
#include "../Resource/ResourcePaths.h"
#include "../System/Actor.h"
#include "../System/AudioManager.h"
#include "../System/GameActivityBase.h"
#include "../System/RenderManager.h"

namespace
{
constexpr float DeathBaseRenderWidth = 64.0f;
constexpr float DeathBaseRenderHeight = 64.0f;

TexturePtr LoadTexture(SDL_Renderer* Renderer, const char* TexturePath)
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

template <size_t N>
std::unique_ptr<Atlas> CreateAtlasFromPaths(SDL_Renderer* Renderer, const std::array<const char*, N>& TexturePaths)
{
    if (Renderer == nullptr)
    {
        return nullptr;
    }

    std::unique_ptr<Atlas> NewAtlas = std::make_unique<Atlas>();
    for (const char* TexturePath : TexturePaths)
    {
        TexturePtr Texture = LoadTexture(Renderer, TexturePath);
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
}

DeathComponent::DeathComponent(Actor& InOwner, AnimationComponent& InAnimationComponent)
    : Owner(InOwner)
    , Animation(InAnimationComponent)
{
}

void DeathComponent::OnDeathBegin()
{
    if (bDying)
    {
        return;
    }

    bDying = true;
    Animation.Stop();
    Owner.SetBaseRenderSize(Vector2D(DeathBaseRenderWidth, DeathBaseRenderHeight));
    AudioManager::GetInstance().PlayAudio(Resources::AudioKey::Explosion);

    std::unique_ptr<Atlas> ExplosionAtlas =
        CreateAtlasFromPaths(RenderManager::GetInstance().GetRenderer(), Resources::Image::Explosion);
    if (!ExplosionAtlas)
    {
        GameActivityBase::GetInstance().DestroyActor(Owner);
        return;
    }

    Animation.SetAtlas(std::move(ExplosionAtlas));
    Animation.Start(false, 1.0f, [this]()
    {
        GameActivityBase::GetInstance().DestroyActor(Owner);
    });
}

bool DeathComponent::IsDying() const
{
    return bDying;
}
