#pragma once
#include <memory>
#include <vector>

#include "Actor.h"

struct SDL_Renderer;

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

private:
    SDL_Renderer* Renderer = nullptr;
};
