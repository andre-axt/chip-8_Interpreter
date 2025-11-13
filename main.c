#include "chip8.h"

int main(){
    Chip8 chip;
    ini_chip8(&chip);
    if(loader_rom(&chip, "teste.rom") == 0){
        
    } 

}