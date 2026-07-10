#include "AudioManager.h"

#include <algorithm>
#include <array>

#include <SDL3/SDL.h>

#include "../Resource/ResourcePaths.h"

bool AudioManager::Initialize()
{
    Shutdown();

    AudioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (AudioDevice == 0)
    {
        return false;
    }

    if (!SDL_GetAudioDeviceFormat(AudioDevice, &DeviceSpec, nullptr))
    {
        Shutdown();
        return false;
    }

    if (!SDL_ResumeAudioDevice(AudioDevice))
    {
        Shutdown();
        return false;
    }

    PreloadAudioData();
    bInitialized = true;
    return true;
}

void AudioManager::Shutdown()
{
    for (ActivePlayback& Playback : ActivePlaybacks)
    {
        if (Playback.Stream != nullptr)
        {
            SDL_DestroyAudioStream(Playback.Stream);
            Playback.Stream = nullptr;
        }
    }

    ActivePlaybacks.clear();
    AudioGroups.clear();

    if (AudioDevice != 0)
    {
        SDL_CloseAudioDevice(AudioDevice);
        AudioDevice = 0;
    }

    DeviceSpec = {};
    bInitialized = false;
}

void AudioManager::Update()
{
    if (!bInitialized)
    {
        return;
    }

    for (ActivePlayback& Playback : ActivePlaybacks)
    {
        if (!Playback.bLoop || Playback.Stream == nullptr || !Playback.Clip)
        {
            continue;
        }

        if (SDL_GetAudioStreamQueued(Playback.Stream) > 0)
        {
            continue;
        }

        SDL_PutAudioStreamData(
            Playback.Stream,
            Playback.Clip->Data.data(),
            static_cast<int>(Playback.Clip->Data.size()));
        SDL_FlushAudioStream(Playback.Stream);
    }

    CleanupFinishedPlaybacks();
}

bool AudioManager::PlayAudio(const char* AudioKey, bool bLoop)
{
    if (!bInitialized || AudioKey == nullptr)
    {
        return false;
    }

    const auto AudioGroupIt = AudioGroups.find(AudioKey);
    if (AudioGroupIt == AudioGroups.end() || AudioGroupIt->second.empty())
    {
        return false;
    }

    const std::vector<std::shared_ptr<LoadedAudioClip>>& AudioGroup = AudioGroupIt->second;
    std::uniform_int_distribution<size_t> ClipDistribution(0, AudioGroup.size() - 1);
    const std::shared_ptr<LoadedAudioClip>& Clip = AudioGroup[ClipDistribution(RandomEngine)];
    ActivePlayback Playback = CreatePlayback(Clip, bLoop);
    if (Playback.Stream == nullptr)
    {
        return false;
    }

    ActivePlaybacks.push_back(std::move(Playback));
    return true;
}

void AudioManager::PreloadAudioData()
{
    RegisterAudioGroup(Resources::AudioKey::Bgm, Resources::Audio::BgmVariants);
    RegisterAudioGroup(Resources::AudioKey::Explosion, Resources::Audio::ExplosionVariants);
    RegisterAudioGroup(Resources::AudioKey::Fire, Resources::Audio::FireVariants);
    RegisterAudioGroup(Resources::AudioKey::Hurt, Resources::Audio::HurtVariants);
    RegisterAudioGroup(Resources::AudioKey::Loss, Resources::Audio::LossVariants);
}

std::shared_ptr<AudioManager::LoadedAudioClip> AudioManager::LoadAudioClip(const char* AudioPath) const
{
    SDL_AudioSpec AudioSpec = {};
    Uint8* AudioBuffer = nullptr;
    Uint32 AudioLength = 0;
    if (!SDL_LoadWAV(AudioPath, &AudioSpec, &AudioBuffer, &AudioLength))
    {
        return nullptr;
    }

    std::shared_ptr<LoadedAudioClip> Clip = std::make_shared<LoadedAudioClip>();
    Clip->Spec = AudioSpec;
    Clip->Data.assign(AudioBuffer, AudioBuffer + AudioLength);
    SDL_free(AudioBuffer);
    return Clip;
}

AudioManager::ActivePlayback AudioManager::CreatePlayback(const std::shared_ptr<LoadedAudioClip>& Clip, bool bLoop) const
{
    ActivePlayback Playback;
    if (!Clip)
    {
        return Playback;
    }

    Playback.Stream = SDL_CreateAudioStream(&Clip->Spec, &DeviceSpec);
    if (Playback.Stream == nullptr)
    {
        return Playback;
    }

    if (!SDL_BindAudioStream(AudioDevice, Playback.Stream))
    {
        SDL_DestroyAudioStream(Playback.Stream);
        Playback.Stream = nullptr;
        return Playback;
    }

    if (!SDL_PutAudioStreamData(Playback.Stream, Clip->Data.data(), static_cast<int>(Clip->Data.size())))
    {
        SDL_DestroyAudioStream(Playback.Stream);
        Playback.Stream = nullptr;
        return Playback;
    }

    if (!SDL_FlushAudioStream(Playback.Stream))
    {
        SDL_DestroyAudioStream(Playback.Stream);
        Playback.Stream = nullptr;
        return Playback;
    }

    Playback.Clip = Clip;
    Playback.bLoop = bLoop;
    return Playback;
}

void AudioManager::CleanupFinishedPlaybacks()
{
    ActivePlaybacks.erase(
        std::remove_if(
            ActivePlaybacks.begin(),
            ActivePlaybacks.end(),
            [](ActivePlayback& Playback)
            {
                if (Playback.Stream == nullptr)
                {
                    return true;
                }

                if (Playback.bLoop || SDL_GetAudioStreamQueued(Playback.Stream) > 0)
                {
                    return false;
                }

                SDL_DestroyAudioStream(Playback.Stream);
                Playback.Stream = nullptr;
                return true;
            }),
        ActivePlaybacks.end());
}
