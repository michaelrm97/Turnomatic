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

#include <chip.h>
#include <compiler.h>

#include <cr_section_macros.h>

#include <pindefs.h>
#include <project_config.h>

#include <gpio.h>
#include <timer.h>
#include <sct.h>
#include <adc.h>

#include <mode.h>

#include <song_table.h>
#include <motor.h>
#include <note_tracker.h>
#include <user.h>
#include <usb.h>

#include <filters.h>

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

    adc_set_periodic(F_SAMPLE, (1 << POT_ADC_CHANNEL) | (1 << MIC_ADC_CHANNEL));

    // Main loop
    while (TRUE) {
    	if (mode == MODE_PLAYING) {
    		track_update();
    		user_update();
    	}
    }

    return 0;
}
