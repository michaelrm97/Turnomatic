/*
===============================================================================
 Name        : auto_turn.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "chip.h"

#include <cr_section_macros.h>

// Hardware include files
#include <gpio.h>
#include <timer.h>
#include <spi.h>

int main(void) {

    SystemCoreClockUpdate();

    gpio_init();
    timer_init();

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}