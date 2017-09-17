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

MODE mode = MODE_STOPPED;

int main(void) {

    // Initialize hardware
    SystemCoreClockUpdate();

    gpio_init();
    timer_init();

//    sct_init();
//    adc_init();
//    uart_init();

    user_init();
    user_mode_set(mode);
//    filter_reset();

	// Interrupt sources:
    // ADC
    // UART
    // Timer
    // PININT 1-5
    while (1) {
    	_U08 x = gpio_getValue(RESTART_BUTTON_PORT, RESTART_BUTTON_PIN);
    	_U08 y = gpio_getValue(PAUSE_BUTTON_PORT, PAUSE_BUTTON_PIN);
    	_U08 z = gpio_getValue(PLAY_BUTTON_PORT, PLAY_BUTTON_PIN);
    	switch(mode) {
    	case MODE_STOPPED:
    	case MODE_PAUSED:
    		break;
    	case MODE_PLAYING:
    		break;
    	case MODE_LOADING:
    		break;
    	}
//		__WFI;
    }

    return 0 ;
}
