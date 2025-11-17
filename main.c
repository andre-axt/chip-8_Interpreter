#include "chip8.h"
#include "stdio.h"
#include "graphic.h"

int main(){
    Chip8 chip;
    ini_SDL();
    ini_chip8(&chip);
    loader_rom(&chip, "test.ch8");
    cycle_chip8(&chip);

    return 0;
}