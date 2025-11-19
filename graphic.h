#ifndef GRAPHIC_H
#define GRAPHIC_H
#include "chip8.h"
#include "graphic.h"
#include <SDL2/SDL>

int ini_SDL();
void update_SDL(Chip8* chip);
int map_key(SDL_Keycode key);
void handle_input();
int get_pressed_key();


#endif