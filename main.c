#include "chip8.h"
#include "stdio.h"
#include "graphic.h"

int main(){
    Chip8 chip;
    ini_chip8(&chip);
    ini_SDL();
    loader_rom(&chip, "test.ch8");
    cycle_chip8(&chip);

    return 0;
}