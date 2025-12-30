#include "chip8.h"
#include <SDL2/SDL.h>

int ini_SDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error ini_SDL: %s\n", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow("Chip-8 Interpreter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * SCALE, HEIGHT * SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    if (*window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        return -1;
    }

    SDL_RaiseWindow(*window);
    SDL_SetWindowInputFocus(*window);
    return 0;
}

void update_SDL(Chip8* chip, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (chip->display[y * WIDTH + x]) {
                SDL_Rect rect = { x * SCALE, y * SCALE, SCALE, SCALE };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}


void update_keys(Chip8 *chip) {
    SDL_PumpEvents();
    
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    
    if(keystate == NULL) {
        printf("ERRO: SDL_GetKeyboardState retornou NULL!\n");
        return;
    }
    static int frame = 0;
    frame++;
    
    if(frame % 60 == 0) {
        printf("SDL Key State Test - Frame %d:\n", frame);
        printf("  1:%d 2:%d 3:%d 4:%d\n", 
               keystate[SDL_SCANCODE_1], keystate[SDL_SCANCODE_2],
               keystate[SDL_SCANCODE_3], keystate[SDL_SCANCODE_4]);
        printf("  Q:%d W:%d E:%d R:%d\n",
               keystate[SDL_SCANCODE_Q], keystate[SDL_SCANCODE_W],
               keystate[SDL_SCANCODE_E], keystate[SDL_SCANCODE_R]);
        printf("  A:%d S:%d D:%d F:%d\n",
               keystate[SDL_SCANCODE_A], keystate[SDL_SCANCODE_S],
               keystate[SDL_SCANCODE_D], keystate[SDL_SCANCODE_F]);
        printf("  Z:%d X:%d C:%d V:%d\n",
               keystate[SDL_SCANCODE_Z], keystate[SDL_SCANCODE_X],
               keystate[SDL_SCANCODE_C], keystate[SDL_SCANCODE_V]);
    }
    
    chip->keys[0x1] = keystate[SDL_SCANCODE_1];  
    chip->keys[0x2] = keystate[SDL_SCANCODE_2];  
    chip->keys[0x3] = keystate[SDL_SCANCODE_3];  
    chip->keys[0xC] = keystate[SDL_SCANCODE_4];  
    chip->keys[0x4] = keystate[SDL_SCANCODE_Q];  
    chip->keys[0x5] = keystate[SDL_SCANCODE_W];  
    chip->keys[0x6] = keystate[SDL_SCANCODE_E];  
    chip->keys[0xD] = keystate[SDL_SCANCODE_R];  
    chip->keys[0x7] = keystate[SDL_SCANCODE_A];  
    chip->keys[0x8] = keystate[SDL_SCANCODE_S];  
    chip->keys[0x9] = keystate[SDL_SCANCODE_D];  
    chip->keys[0xE] = keystate[SDL_SCANCODE_F];  
    chip->keys[0xA] = keystate[SDL_SCANCODE_Z];  
    chip->keys[0x0] = keystate[SDL_SCANCODE_X];  
    chip->keys[0xB] = keystate[SDL_SCANCODE_C];  
    chip->keys[0xF] = keystate[SDL_SCANCODE_V];  
    
    if(frame % 30 == 0) {
        printf("CHIP8 Keys: ");
        int count = 0;
        for(int i = 0; i < 16; i++) {
            if(chip->keys[i]) {
                printf("%X(%d) ", i, chip->keys[i]);
                count++;
            }
        }
        if(count == 0) printf("NONE");
        printf("\n");
    }
}
