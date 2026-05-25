#ifndef GRAPHIC_H
#define GRAPHIC_H
#include "chip8.h"
#include "graphic.h"
#include <SDL2/SDL.h>

int ini_SDL(SDL_Window** window, SDL_Renderer** renderer);
void update_SDL(Chip8* chip, SDL_Renderer* renderer);
void update_keys(Chip8 *chip);

#endif
