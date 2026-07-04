#pragma once
#include <memory>
#include <vector>
#include <SDL3/SDL_render.h>

struct SDLTextureDeleter
{
    void operator()(SDL_Texture* Texture) const noexcept;
};

using TexturePtr = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

//图集类存储图集

class Atlas
{
public:
    Atlas() = default;
    explicit Atlas(TexturePtr InitialFrame);

    void AddFrame(TexturePtr FrameTexture);
    void ClearFrames();
    void Reset();

    bool AdvanceFrame(bool bLoop);
    bool RenderCurrentFrame(SDL_Renderer& Renderer, const SDL_FRect& DestRect) const;
    bool IsFinished() const;
    bool IsValid() const;

private:
    std::vector<TexturePtr> Frames;
    size_t CurrentFrameIndex = 0;
    bool bFinished = false;
};
