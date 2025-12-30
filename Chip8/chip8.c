#include "chip8.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "graphic.h"

void ini_chip8(Chip8* chip){
    memset(chip->memory, 0, MEMORY);
    chip->pc = 0x200;
    memset(chip->V, 0, sizeof(NUM_REGISTERS));
    memset(chip->display, 0, sizeof(chip->display));
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
    memcpy(&chip->memory[0x50], chip8_fontset, 80);

    printf("Font loaded at 0x50: ");
    for(int i = 0; i < 10; i++) {
        printf("%02X ", chip->memory[0x50 + i]);
    }
    printf("\n");
}

int loader_rom(Chip8* chip, char *filename){
    FILE *file = fopen(filename, "rb");
    if(file == NULL){
        printf("Error opening file!");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    if(file_size > MEMORY){
        printf("file_size > memory");
        return 1;
    }
    rewind(file);

    int bytes_read = fread(chip->memory + 0x200, 1, file_size, file);
    fclose(file);
    return 0;
}

void cycle_chip8(Chip8 *chip, SDL_Renderer* renderer){
    chip->op = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];
    uint8_t Vx = (chip->op & 0x0F00) >> 8;
    uint8_t Vy = (chip->op & 0x00F0) >> 4;
    uint8_t kk = chip->op & 0x00FF;
    uint16_t next_pc = chip->pc + 2;
    switch (chip->op & 0xF000)
    {
    case 0x0000:
        switch (chip->op)
        {
        case 0x00E0:
            memset(chip->display, 0, sizeof(chip->display));
            chip->draw_flag = 1;
            break;

        case 0x00EE:
            next_pc = chip->stack[chip->sp];
	    chip->sp--;
            break;
        
        default:
            break;
        }
        break;
    
    case 0x1000:
        next_pc = chip->op & 0x0FFF;
        break;

    case 0x2000:
	chip->sp++;
        chip->stack[chip->sp] = chip->pc;
        next_pc = chip->op & 0X0FFF;
        break;

    case 0x3000:
        if(chip->V[Vx] == kk){
            next_pc += 2;
        }
        break;

    case 0x4000:
        if(chip->V[Vx] != kk){
            next_pc += 2;
        }
        break;

    case 0x5000:
        if(chip->V[Vx] == chip->V[Vy]){
            next_pc += 2;
        }
        break;
    
    case 0x6000:
        chip->V[Vx] = kk;
        break;

    case 0x7000:
        chip->V[Vx] = chip->V[Vx] + kk;
        break;

    case 0x8000:
        switch (chip->op & 0xF00F)
        {
        case 0x8000:
            chip->V[Vx] = chip->V[Vy];
            break;
        
        case 0x8001:
            chip->V[Vx] = chip->V[Vx] | chip->V[Vy];
            break;

        case 0x8002:
            chip->V[Vx] = chip->V[Vx] & chip->V[Vy];
            break;
        
        case 0x8003:
            chip->V[Vx] = chip->V[Vx] ^ chip->V[Vy];
            break;
        
        case 0x8004: 
            uint16_t sum = chip->V[Vx] + chip->V[Vy];
            if(sum > 0xFF){
                chip->V[0xF] = 1;
            }else{
                chip->V[0xF] = 0;
            }
            chip->V[Vx] = sum & 0xFF;
            break;

        case 0x8005:
            if(chip->V[Vx] >= chip->V[Vy]){
                chip->V[0xF] = 1;
            }else{
                chip->V[0xF] = 0;
            }
            chip->V[Vx] = chip->V[Vx] - chip->V[Vy];
                break;

        case 0x8006:
            chip->V[0xF] = chip->V[Vx] & 0x01;
	    chip->V[Vx] >>= 1;
            break;
        
        case 0x8007:
            if(chip->V[Vy] >= chip->V[Vx]){
                chip->V[0xF] = 1;
            }else{
                chip->V[0xF] = 0;
            }
            chip->V[Vx] = chip->V[Vx] - chip->V[Vy];
            break;
        
        case 0x800E:
            chip->V[0xF] = (chip->V[Vx] >> 7) & 0x01;
            chip->V[Vx] <<= 1;
            break;


        default:
            break;
        }
        break;
        

    case 0x9000:
        if(chip->V[Vx] != chip->V[Vy]){
            next_pc += 2;
        }
        break;

    case 0xA000:
        chip->I = chip->op & 0x0FFF;
        break;

    case 0xB000:
        next_pc = (chip->op & 0x0FFF) + chip->V[0];
        break;

    case 0xC000:
        chip->V[Vx] = (rand() % 256) & kk;
        break;

    case 0xD000:
        uint8_t n = chip->op & 0x000F;
        uint8_t x_pos = chip->V[Vx] % WIDTH;
        uint8_t y_pos = chip->V[Vy] % HEIGHT;
        chip->V[0xF] = 0;

        for(int row = 0; row < n; row++){
            uint8_t sprite_byte = chip->memory[chip->I + row];
            for(int col = 0; col < 8; col++){
                if (sprite_byte & (0x80 >> col)){
                    uint8_t final_x = (x_pos + col) % WIDTH;
                    uint8_t final_y = (y_pos + row) % HEIGHT;

                    uint16_t display_index = final_y * WIDTH + final_x;

                    if(chip->display[display_index] == 1){
                        chip->V[0xF] = 1;
                    }

                    chip->display[display_index] ^= 1;

                }
            }
        }
        chip->draw_flag = 1;
        printf("DXYN: op=%04X Vx=%d Vy=%d n=%d x_pos=%d y_pos=%d\n", chip->op, Vx, Vy, n, x_pos, y_pos);
        printf("Drawing sprite at (%d,%d) with %d rows from I=%03X\n", x_pos, y_pos, n, chip->I);
        break;
    
    case 0xE000:
        switch (chip->op & 0xF0FF)
        {
        case 0xE09E:
            if(chip->keys[chip->V[Vx]] == 1){
                next_pc += 2;
            }
            break;
        
        case 0xE0A1:
            if(chip->keys[chip->V[Vx]] == 0){
                next_pc += 2;
            }
            break;

        default:
            break;
        }
        break;

    case 0xF000:
        switch (chip->op & 0xF0FF)
        {
        case 0xF007:
            chip->V[Vx] = chip->delay_timer;
            break;

        case 0xF00A:
            int key_pressed = -1;
    	    for (int i = 0; i < 16; i++) {
            	if (chip->keys[i]) {
               	key_pressed = i;
                break;
                }
            }
    
    	    if (key_pressed != -1) {
            	chip->V[Vx] = key_pressed;
    	    } else {
        	next_pc = chip->pc;
            }
    	    break;

        case 0xF015:
            chip->delay_timer = chip->V[Vx];
            break;

        case 0xF018:
            chip->sound_timer = chip->V[Vx];
            break;

        case 0xF01E:
            chip->I += chip->V[Vx];
            break;

        case 0xF029:
            chip->I = 0x50 + (chip->V[Vx] & 0xF) * 5;
            break;

        case 0xF033:
            chip->memory[chip->I] = chip->V[Vx] / 100;
            chip->memory[chip->I + 1] = (chip->V[Vx] % 100) / 10;
            chip->memory[chip->I + 2] = (chip->V[Vx] % 10);
            break;

        case 0xF055:
            for(int i = 0; i <= Vx; i++){
                chip->memory[chip->I + i] = chip->V[i];
            }
            break;

        case 0xF065:
            for(int i = 0; i <= Vx; i++){
                chip->V[i] = chip->memory[chip->I + i];
            }
            break;
            
        default:
            break;
        }
        break;

    default:
        break;
    }
    printf("Opcode: %X at PC: %X\n", chip->op, chip->pc);
    chip->pc = next_pc;
    if(chip->draw_flag == 1){
        update_SDL(chip, renderer);
    }
    chip->draw_flag = 0;
}
