#ifndef GRAPHIC_H
#define GRAPHIC_H
#include "chip8.h"
#include "graphic.h"
#include <SDL2/SDL.h>

int ini_SDL(SDL_Window** window, SDL_Renderer** renderer);
void update_SDL(Chip8* chip, SDL_Renderer* renderer);
int map_key(SDL_Keycode key);
void handle_input(Chip8 *chip, SDL_Event* event);
int get_pressed_key(Chip8 *chip);


#endif