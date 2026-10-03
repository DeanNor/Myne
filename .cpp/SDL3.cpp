
#include "SDL3.h"

#include <cassert>
#include <filesystem>

#include "err.hpp"
#include "game.hpp"

#include <SDL3_image/SDL_image.h>

// Should not be called in the draw function as it clears the content of renderer (causes lag, not actual problem), although YOLO // TODO add to .h not this
// Saves the texture to path as a png, but does not add the .png to the end
// Texture must be attached to renderer
void save_img(SDL_Texture* texture, SDL_Renderer* renderer, std::filesystem::path path)
{
    int width = texture->w, height = texture->h;
    SDL_PixelFormat format = texture->format;

    SDL_Texture* target = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, width, height);

    SDL_SetRenderTarget(renderer, target);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);

    SDL_Surface* surface = SDL_RenderReadPixels(renderer, nullptr);

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_DestroyTexture(target);

    IMG_SavePNG(surface, path.generic_string().c_str());

    SDL_DestroySurface(surface);
}

// Loads texture from path. The texture will be attached to the renderer
SDL_Texture* load_img(SDL_Renderer* renderer, std::filesystem::path path, SDL_ScaleMode scale_mode)
{
    ASSERT(std::filesystem::exists(path), std::string("File path does not exist ") + path.generic_string());

    SDL_Texture* texture = IMG_LoadTexture(renderer, path.generic_string().c_str());
    
    ASSERT(texture, "ERROR with texture creation");

    return texture;
}

void play_audio(std::filesystem::path path, game* game, float gain)
{
    audio_type audio = load_audio(path);
    play_audio(audio, game, gain);
    audio.free_data();
}

void play_audio(audio_type audio, game* game, float gain)
{
    SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio.spec, NULL, NULL);

    SDL_PutAudioStreamData(stream, audio.wav_data, audio.wav_data_len);

    SDL_SetAudioStreamGain(stream, gain);

    SDL_ResumeAudioStreamDevice(stream);

    game->add_audio_stream(stream);
}

audio_type load_audio(std::filesystem::path path)
{
    audio_type v;
    SDL_LoadWAV(path.string().data(), &v.spec, &v.wav_data, &v.wav_data_len);
    return v;
}