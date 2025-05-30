
#pragma once

#include <SDL3/SDL.h>

#include <SDL3_image/SDL_image.h>

#include <string>

void save_img(SDL_Texture* texture, SDL_Renderer* renderer, std::string name);

void load_img(SDL_Texture*& texture, SDL_Renderer* renderer, std::string name);