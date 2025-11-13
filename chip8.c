#include "chip8.h"
#include "stdio.h"
#include "stdlib.h"

void ini_chip8(Chip8* chip){
    memset(chip->memory, 0, sizeof(MEMORY));
    chip->pc = 0x200;
    memset(chip->registers, 0, sizeof(NUM_REGISTERS));
    memset(chip->display, 0, sizeof(WIDTH * HEIGHT));
    chip->I = 0;
    chip->sp = 0;
    chip->draw_flag = 0;
    chip->sound_timer = 0;
    chip->delay_timer = 0;
    memset(chip->stack, 0, sizeof(STACK));
    memset(chip->keys, 0, sizeof(NUM_KEYS));
    load_fontset(chip);

}

void load_fontset(Chip8* chip){
    for(int i = 0; i < 80; i++){
        memset(chip->memory[0x50 + 5 * i], i , sizeof(MEMORY));
    }
}

int loader_rom(Chip8* chip, char *filename){
    FILE *file = fopen(filename, "rb");
    if(file == NULL){
        printf("Error opening file!");
    }

    int bytes_read = fread(chip->memory + 0x200, 1, 4024, file);
    fclose(file);
    return 0;
}