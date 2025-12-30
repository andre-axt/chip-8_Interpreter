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
    loader_rom(&chip, "");
    int running = 1;
    SDL_Event event;
    const double timer_update = 1000 / 60;
    int now = SDL_GetPerformanceCounter();
    int last_time = 0;
    double delta_time = 0;
    while (running){
	while(SDL_PollEvent(&event)) {    
        	if(event.type == SDL_QUIT){
        		running = 0;
		}
	}
        
	update_keys(&chip);
        now = SDL_GetPerformanceCounter();
        delta_time += (double)((now - last_time) * 1000 / (double)SDL_GetPerformanceFrequency());
        last_time = now;

        if (delta_time >= timer_update){
            if(chip.delay_timer > 0){
                chip.delay_timer--;
            }
            if(chip.sound_timer > 0){
                chip.sound_timer--;

            }
            delta_time -= timer_update;
        }
        	
        cycle_chip8(&chip, renderer);
	SDL_Delay(2);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
