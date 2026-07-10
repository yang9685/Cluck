#pragma once

#include <array>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL3/SDL_audio.h>

class AudioManager
{
private:
    struct LoadedAudioClip
    {
        SDL_AudioSpec Spec = {};
        std::vector<Uint8> Data;
    };

    struct ActivePlayback
    {
        std::shared_ptr<LoadedAudioClip> Clip;
        SDL_AudioStream* Stream = nullptr;
        bool bLoop = false;
    };

    AudioManager() = default;
    ~AudioManager() = default;

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

public:
    static AudioManager& GetInstance()
    {
        static AudioManager Instance;
        return Instance;
    }

    bool Initialize();
    void Shutdown();
    void Update();
    bool PlayAudio(const char* AudioKey, bool bLoop = false);

private:
    template <size_t N>
    void RegisterAudioGroup(const char* AudioKey, const std::array<const char*, N>& AudioPaths)
    {
        std::vector<std::shared_ptr<LoadedAudioClip>>& AudioGroup = AudioGroups[AudioKey];
        AudioGroup.clear();

        for (const char* AudioPath : AudioPaths)
        {
            std::shared_ptr<LoadedAudioClip> Clip = LoadAudioClip(AudioPath);
            if (Clip)
            {
                AudioGroup.push_back(std::move(Clip));
            }
        }
    }

    void PreloadAudioData();
    std::shared_ptr<LoadedAudioClip> LoadAudioClip(const char* AudioPath) const;
    ActivePlayback CreatePlayback(const std::shared_ptr<LoadedAudioClip>& Clip, bool bLoop) const;
    void CleanupFinishedPlaybacks();

private:
    SDL_AudioDeviceID AudioDevice = 0;
    SDL_AudioSpec DeviceSpec = {};
    std::unordered_map<std::string, std::vector<std::shared_ptr<LoadedAudioClip>>> AudioGroups;
    std::vector<ActivePlayback> ActivePlaybacks;
    std::mt19937 RandomEngine = std::mt19937(std::random_device{}());
    bool bInitialized = false;
};
