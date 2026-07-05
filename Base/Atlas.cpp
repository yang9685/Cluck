#include "Atlas.h"

void SDLTextureDeleter::operator()(SDL_Texture* Texture) const noexcept
{
    if (Texture != nullptr)
    {
        SDL_DestroyTexture(Texture);
    }
}

Atlas::Atlas(TexturePtr InitialFrame)
{
    AddFrame(std::move(InitialFrame));
}

void Atlas::AddFrame(TexturePtr FrameTexture)
{
    if (FrameTexture)
    {
        Frames.push_back(std::move(FrameTexture));
    }
}

void Atlas::ClearFrames()
{
    Frames.clear();
    Reset();
}

void Atlas::Reset()
{
    CurrentFrameIndex = 0;
    bFinished = false;
}

bool Atlas::AdvanceFrame(bool bLoop)
{
    if (Frames.empty())
    {
        return false;
    }

    if (bFinished && !bLoop)
    {
        return false;
    }

    if (CurrentFrameIndex + 1 < Frames.size())
    {
        ++CurrentFrameIndex;
        return true;
    }

    if (bLoop)
    {
        CurrentFrameIndex = 0;
        bFinished = false;
        return true;
    }

    CurrentFrameIndex = Frames.size() - 1;
    bFinished = true;
    return false;
}

SDL_Texture* Atlas::GetCurrentFrameTexture() const
{
    if (Frames.empty())
    {
        return nullptr;
    }

    const TexturePtr& CurrentFrame = Frames[CurrentFrameIndex];
    if (!CurrentFrame)
    {
        return nullptr;
    }

    return CurrentFrame.get();
}

bool Atlas::IsFinished() const
{
    return bFinished;
}

bool Atlas::IsValid() const
{
    return !Frames.empty();
}
