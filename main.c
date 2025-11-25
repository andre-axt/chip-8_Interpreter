#include "chip8.h"
#include "stdio.h"
#include "graphic.h"
#include <SDL2/SDL.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int main(){
    Chip8 chip;
    ini_chip8(&chip);
    ini_SDL(&window, &renderer);
    loader_rom(&chip, "test.ch8");
    load_fontset(&chip);
    int running = 1;
    SDL_Event event;
    while (running){
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                running = 0;
            }
            handle_input(&chip, &event);
        }
        
        cycle_chip8(&chip, renderer);
        SDL_Delay(chip.delay);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
