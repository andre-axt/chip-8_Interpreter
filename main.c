#include "chip8.h"
#include "stdio.h"

int main(){
    Chip8 chip;
    ini_chip8(&chip);
    if(loader_rom(&chip, "test.ch8") == 0){
        printf("Ok");
        printf("Primeiros bytes da ROM:\n");
        
        for (int i = 0; i < 10; i++) {
            printf("0x%02X ", chip.memory[0x200 + i]);
        }
        printf("\n");
    } 
    return 0;
}