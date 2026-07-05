#pragma once
#include <memory>
#include <vector>
#include <SDL3/SDL_render.h>

struct SDLTextureDeleter
{
    void operator()(SDL_Texture* Texture) const noexcept;
};

using TexturePtr = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

class Atlas
{
public:
    Atlas() = default;
    explicit Atlas(TexturePtr InitialFrame);

    void AddFrame(TexturePtr FrameTexture);
    void ClearFrames();
    void Reset();

    bool AdvanceFrame(bool bLoop);
    SDL_Texture* GetCurrentFrameTexture() const;
    bool IsFinished() const;
    bool IsValid() const;

private:
    std::vector<TexturePtr> Frames;
    size_t CurrentFrameIndex = 0;
    bool bFinished = false;
};
