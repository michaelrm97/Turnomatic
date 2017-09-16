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
#include <sct.h>
#include <adc.h>

#include <pmoleds.h>
#include <user.h>
#include <motor.h>
#include <usb.h>
#include <filters.h>
#include <song_table.h>

#include <mode.h>

MODE mode = MODE_STOPPED;

int main(void) {

    // Initialize hardware
    SystemCoreClockUpdate();

    gpio_init();
    timer_init();
    sct_init();
    adc_init();
//    uart_init();

    user_init();
    filter_reset();

	// Interrupt sources:
    // ADC
    // UART
    // Timer
    // PININT 1-5
    while (1) {
    	switch(mode) {
    	case MODE_STOPPED:
    	case MODE_PAUSED:
    		break;
    	case MODE_PLAYING:
    		break;
    	case MODE_LOADING:
    		break;
    	}
		__WFI;
    }

    return 0 ;
}
