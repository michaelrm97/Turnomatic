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

#include <pindefs.h>
#include <project_config.h>

// Hardware include files
#include <gpio.h>
#include <timer.h>
#include <sct.h>
#include <adc.h>

#include <pm_graphics.h>
#include <pmoleds.h>
#include <user.h>
#include <motor.h>
#include <usb.h>
#include <filters.h>
#include <song_table.h>

#include <mode.h>

MODE mode;

int main(void) {

    // Initialize hardware
    SystemCoreClockUpdate();

    gpio_init();
    timer_init();

//    sct_init();
//    adc_init();
//    uart_init();

    song_table_init();

    user_init();
//    usb_init();
//    filter_reset();

	// Interrupt sources:
    // ADC
    // UART
    // Timer
    // PININT 1-5
    int i = 0;
    while (1) {
		i++;
    }

    return 0 ;
}
