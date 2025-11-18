#include "chip8.h"
#include "stdio.h"
#include "graphic.h"

int main(){
    Chip8 chip;
    ini_SDL();
    ini_chip8(&chip);
    loader_rom(&chip, "test.ch8");
    while(1){
        if(!waiting_for_key){
            cycle_chip8(&chip);

        }else {
            check_key_press();
        }
    }

    return 0;
}