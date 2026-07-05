#pragma once

#include "../Base/Atlas.h"
#include "../System/Actor.h"

class BackgroundActor : public Actor
{
public:
    BackgroundActor();

    void Render(SDL_Renderer* Renderer) override;

private:
    bool EnsureTextureLoaded(SDL_Renderer* Renderer);

private:
    TexturePtr BackgroundTexture;
};
