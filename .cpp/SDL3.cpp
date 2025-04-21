
#include "SDL3.h"

void save_img(SDL_Texture* texture, std::string path)
{
    SDL_Surface* surf;
    SDL_LockTextureToSurface(texture, nullptr, &surf);
    IMG_SavePNG(surf, path.c_str());
    SDL_UnlockTexture(texture);
}

void load_img(SDL_Texture* texture, SDL_Renderer* renderer, std::string path)
{
    SDL_Surface* surf = IMG_Load(path.c_str());

    texture = SDL_CreateTextureFromSurface(renderer, surf);\
    
    SDL_DestroySurface(surf);
}