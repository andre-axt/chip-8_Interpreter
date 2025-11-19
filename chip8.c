#include "chip8.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "graphic.h"

void ini_chip8(Chip8* chip){
    memset(chip->memory, 0, sizeof(chip->memory));
    chip->pc = 0x200;
    memset(chip->V, 0, sizeof(chip->V));
    memset(chip->display, 0, sizeof(chip->display));
    chip->I = 0;
    chip->sp = 0;
    chip->draw_flag = 0;
    chip->sound_timer = 0;
    chip->delay_timer = 0;
    memset(chip->stack, 0, sizeof(chip->stack));
    memset(chip->keys, 0, sizeof(chip->keys));
    load_fontset(chip);

}

void load_fontset(Chip8* chip){
    memcpy(&chip->memory[0x50], chip8_fontset, 80);
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

void cycle_chip8(Chip8 *chip){
    while(1){
        handle_input(chip);
        chip->op = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];
        uint8_t Vx = (chip->op & 0x0F00) >> 8;
        uint8_t Vy = (chip->op & 0x00F0) >> 4;
        uint8_t kk = chip->op & 0x00FF;
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
                chip->pc = chip->stack[0];
                chip->sp--;
            
            default:
                break;
            }
            break;
        
        case 0x1000:
            chip->pc = chip->op & 0x0FFF;
            break;
    
        case 0x2000:
            chip->sp++;
            chip->stack[0] = chip->pc;
            chip->pc = chip->op & 0X0FFF;
            break;
    
        case 0x3000:
            if(Vx == kk){
                chip->pc += 2;
            }
            break;
    
        case 0x4000:
            if(Vx != kk){
                chip->pc += 2;
            }
            break;
    
        case 0x5000:
            if(Vx == Vy){
                chip->pc += 2;
            }
            break;
        
        case 0x6000:
            Vx = kk;
            break;
    
        case 0x7000:
            Vx = Vx + kk;
            break;
    
        case 0x8000:
            switch (chip->op & 0xF00F)
            {
            case 0x8000:
                Vx = Vy;
                break;
            
            case 0x8001:
                Vx = Vx | Vy;
                break;
    
            case 0x8002:
                Vx = Vx & Vy;
                break;
            
            case 0x8003:
                Vx = Vx ^ Vy;
                break;
            
            case 0x8004: // *
                Vx = Vx + Vy;
                if(Vx > 0xFF){
                    chip->V[15] = 1;
                }else{
                    chip->V[15] = 0;
                }
                break;
    
            case 0x8005:
                Vx = Vx - Vy;
                if(Vx > Vy){
                    chip->V[15] = 1;
                }else{
                    chip->V[15] = 0;
                }
                break;
    
            case 0x8006: /**/
                if((Vx << 7) == 0x80){
                    chip->V[15] = 1;
                }else{
                    chip->V[15] = 0;
                }
                Vx = Vx >> 1;
                break;
            
            case 0x8007:
                if(Vy > Vx){
                    chip->V[15] = 1;
                }else{
                    chip->V[15] = 0;
                }
                Vx = Vx - Vy;
                break;
            
            case 0x800E:
                if((Vx >> 7) == 1){
                    chip->V[15] = 1;
                }else{
                    chip->V[15] = 0;
                }
                Vx = Vx << 1;
                break;
    
    
            default:
                break;
            }
            
    
        case 0x9000:
            if(Vx != Vy){
                chip->pc += 2;
            }
    
        case 0xA000:
            chip->I = chip->op & 0x0FFF;
    
        case 0xB000:
            chip->pc = (chip->op & 0x0FFF) + chip->V[0];
    
        case 0xC000:
            uint8_t random_byte = rand() % 256;
            Vx = random_byte & kk;
    
        case 0xD000:
            uint8_t n = chip->op & 0x000F;
            uint8_t x_pos = chip->V[Vx] % WIDTH;
            uint8_t y_pos = chip->V[Vy] % HEIGHT;
            chip->V[15] = 0;
    
            for(int row = chip->I; row < n; row++){
                uint8_t sprite_byte = chip->memory[row];
                for(int col = 0; col < WIDTH; col++){
                    if (sprite_byte & (0x80 >> col)){
                        uint8_t final_x = (x_pos + col) % WIDTH;
                        uint8_t final_y = (y_pos + row) % HEIGHT;
    
                        uint16_t display_index = final_y * WIDTH + final_x;
    
                        if(chip->display[display_index] == 1){
                            chip->V[15] = 1;
                        }
    
                        chip->display[display_index] ^= 1;
    
                    }
                }
            }
            chip->draw_flag = 1;
        
        case 0xE000:
            switch (chip->op & 0xF0FF)
            {
            case 0xE09E:
                if(chip->keys[chip->V[Vx]] == 1){
                    chip->pc += 2;
                }
                break;
            
            case 0xE0A1:
                if(chip->keys[chip->V[Vx]] == 0){
                    chip->pc += 2;
                }
                break;
    
            default:
                break;
            }
    
        case 0xF000:
            switch (chip->op & 0xF0FF)
            {
            case 0xF007:
                chip->delay_timer = Vx;
                break;
    
            case 0xF00A:
                uint8_t key = get_pressed_key();
                if(key != -1){
                    chip->V[Vx] = key;
                }
                break;
    
            case 0xF015:
                chip->delay_timer = Vx;
                break;
    
            case 0xF018:
                chip->sound_timer = Vx;
                break;
    
            case 0xF01E:
                chip->I += Vx;
                break;
    
            case 0xF029:
                chip->I = 0x50 + (chip->V[Vx] & 0xF) * 5;
                break;
    
            
            default:
                break;
            }
    
        default:
            break;
        }
        printf("Opcode: %X at PC: %X\n", chip->op, chip->pc);
        printf("Drawing sprite at (%d,%d) with %d rows from I=%X\n", Vx, Vy, kk, chip->I);
        if(!(chip->op & 0xF000 == 0x1000 || chip->op & 0xF000 == 0x2000 || chip->op & 0x00FF == 0x00EE || chip->op & 0xF000 == 0xB000)){
            chip->pc += 2;
        }
        if(chip->draw_flag == 1){
            update_SDL(chip);
        }
    }
}