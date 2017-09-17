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
    pm_init();

    pm_draw_rectangle(10, 10, 20, 20);
    pm_write_buffer();

    gpio_setPinDir(LED_PAUSED_PORT, LED_PAUSED_PIN, GPIO_DIR_OUTPUT);
    gpio_setPinMode(LED_PAUSED_PORT, LED_PAUSED_PIN, GPIO_MODE_NONE);

    gpio_setPinDir(LED_RUNNING_PORT, LED_RUNNING_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(LED_RUNNING_PORT, LED_RUNNING_PIN, GPIO_MODE_NONE);

	gpio_setPinDir(LED_STOPPED_PORT, LED_STOPPED_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(LED_STOPPED_PORT, LED_STOPPED_PIN, GPIO_MODE_NONE);

	gpio_setPinDir(BUCK_EN_PORT, BUCK_EN_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(BUCK_EN_PORT, BUCK_EN_PIN, GPIO_MODE_NONE);

	gpio_setPinDir(RESTART_BUTTON_PORT, RESTART_BUTTON_PIN, GPIO_DIR_INPUT);
	gpio_setPinMode(RESTART_BUTTON_PORT, RESTART_BUTTON_PIN, GPIO_MODE_PULLUP);

	gpio_setPinDir(PAUSE_BUTTON_PORT, PAUSE_BUTTON_PIN, GPIO_DIR_INPUT);
	gpio_setPinMode(PAUSE_BUTTON_PORT, PAUSE_BUTTON_PIN, GPIO_MODE_PULLUP);

	gpio_setPinDir(PLAY_BUTTON_PORT, PLAY_BUTTON_PIN, GPIO_DIR_INPUT);
	gpio_setPinMode(PLAY_BUTTON_PORT, PLAY_BUTTON_PIN, GPIO_MODE_PULLUP);

	gpio_setPinDir(UP_BUTTON_PORT, UP_BUTTON_PIN, GPIO_DIR_INPUT);
	gpio_setPinMode(UP_BUTTON_PORT, UP_BUTTON_PIN, GPIO_MODE_PULLUP);

	gpio_setPinDir(DOWN_BUTTON_PORT, DOWN_BUTTON_PIN, GPIO_DIR_INPUT);
	gpio_setPinMode(DOWN_BUTTON_PORT, DOWN_BUTTON_PIN, GPIO_MODE_PULLUP);

//    sct_init();
//    adc_init();
//    uart_init();

//    user_init();
//    filter_reset();

    while (1) {
		gpio_setPinValue(LED_STOPPED_PORT, LED_STOPPED_PIN,
				!gpio_getValue(RESTART_BUTTON_PORT, RESTART_BUTTON_PIN));

		gpio_setPinValue(LED_PAUSED_PORT, LED_PAUSED_PIN,
				!gpio_getValue(PAUSE_BUTTON_PORT, PAUSE_BUTTON_PIN));

		gpio_setPinValue(LED_RUNNING_PORT, LED_RUNNING_PIN,
				!gpio_getValue(PLAY_BUTTON_PORT, PLAY_BUTTON_PIN));

		gpio_setPinValue(BUCK_EN_PORT, BUCK_EN_PIN,
				!gpio_getValue(UP_BUTTON_PORT, UP_BUTTON_PIN) ||
				!gpio_getValue(DOWN_BUTTON_PORT, DOWN_BUTTON_PIN));

    }

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
