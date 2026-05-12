
#pragma once

#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"

#include <filesystem>

struct game;

void save_img(SDL_Texture* texture, SDL_Renderer* renderer, std::filesystem::path path);

[[nodiscard]]
SDL_Texture* load_img(SDL_Renderer* renderer, std::filesystem::path path, SDL_ScaleMode scale_mode = SDL_SCALEMODE_PIXELART);

// Gain between 0.0f as no sound and inf. 1.0f is standard gain.
void play_audio(std::filesystem::path path, game* game, float gain = 1.0);

// Data must be freed with a call to free_data();
struct audio_type
{
    Uint8* wav_data;
    Uint32 wav_data_len;
    SDL_AudioSpec spec;

    void free_data()
    {
        SDL_free(wav_data);
    }
};

// Gain between 0.0f as no sound and inf. 1.0f is standard gain.
void play_audio(audio_type audio, game* game, float gain = 1.0);

[[nodiscard]]
audio_type load_audio(std::filesystem::path path);