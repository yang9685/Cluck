#include "RenderManager.h"

#include <algorithm>
#include <SDL3/SDL_render.h>

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

