
#include "SDL3.h"

#include <cassert>
#include <filesystem>

#include "err.hpp"

// Should not be called in the draw function as it clears the content of renderer, although YOLO
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

    IMG_SavePNG(surface, path.c_str());

    SDL_DestroySurface(surface);
}

// Loads texture from path. The texture will be attached to the renderer
void load_img(SDL_Texture*& texture, SDL_Renderer* renderer, std::filesystem::path path)
{
    ASSERT(std::filesystem::exists(path), "File path does not exist");

    SDL_Surface* surf = IMG_Load(path.c_str());

    texture = SDL_CreateTextureFromSurface(renderer, surf);
    
    SDL_DestroySurface(surf);
}