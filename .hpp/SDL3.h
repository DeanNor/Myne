
#pragma once

#include <SDL3/SDL.h>

#include <SDL3_image/SDL_image.h>

#include <filesystem>

void save_img(SDL_Texture* texture, SDL_Renderer* renderer, std::filesystem::path path);

void load_img(SDL_Texture*& texture, SDL_Renderer* renderer, std::filesystem::path path);