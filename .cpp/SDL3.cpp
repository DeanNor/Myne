
#include ".hpp/SDL3.h"

#include <SDL3_image/SDL_image.h>
#include <cassert>
#include <filesystem>

#include ".hpp/err.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"

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

    IMG_SavePNG(surface, path.generic_string().c_str());

    SDL_DestroySurface(surface);
}

// Loads texture from path. The texture will be attached to the renderer
void load_img(SDL_Texture*& texture, SDL_Renderer* renderer, std::filesystem::path path, SDL_ScaleMode scale_mode)
{
    ASSERT(std::filesystem::exists(path), std::string("File path does not exist ") + path.generic_string());

    texture = IMG_LoadTexture(renderer, path.generic_string().c_str());
    
    ASSERT(texture, "ERROR with texture creation");

    SDL_SetTextureScaleMode(texture, scale_mode);
}