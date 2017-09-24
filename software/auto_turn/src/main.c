/*
===============================================================================
 Name        : main.c
 Author      : Michael Manansala
 Description : Code for an automatic sheet music page turner
 	 	 	   Interfaces with peripherals on Turnomatic PCB
 	 	 	   and performs DSP on sound signal to track progress through
 	 	 	   sheet music
 	 	 	   Special thanks to Thomas Dransfield whose Toucan for the LPC54101
 	 	 	   is what much of the LPCOpen Abstraction in this code base is
 	 	 	   based on
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
#include <note_tracker.h>

#include <mode.h>

MODE mode;

int main(void) {

    // Initialize hardware
    SystemCoreClockUpdate();

    gpio_init();
    timer_init();
    adc_init();

    song_table_init();
    motor_init();
    track_init();
    user_init();
    usb_init();

    track_begin();

//    gpio_setPinValue(BUCK_EN_PORT, BUCK_EN_PIN, 1); // Turn on buck
//    gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 1); // Turn on buck

	// Interrupt sources:
    // ADC
    // UART
    // Timer
    // PININT 1-5
    while (1);

    return 0 ;
}
