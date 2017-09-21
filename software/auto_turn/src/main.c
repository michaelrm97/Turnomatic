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

static void num2str(int n, char *s) {
	if (n == 0) {
		s[0] = '0';
		s[1] = '\0';
		return;
	}
	int num = n;
	int digits = 0;
	while (num) {
		digits++;
		num /= 10;
	}
	s[digits--] = '\0';
	while (digits >= 0) {
		s[digits--] = (n % 10) + '0';
		n /= 10;
	}
}

int main(void) {

    // Initialize hardware
    SystemCoreClockUpdate();

    gpio_init();
    timer_init();

    sct_init();
    adc_init();
    motor_init();
//    uart_init();
//    track_init();

    song_table_init();

    user_init();
//    adc_pinassign(POT_ADC_PORT, POT_ADC_PIN);

//    usb_init();
//    filter_reset();
//    pm_init();

    sct_pwm_setPinDutyCycle(MOTOR1_SCT_INDEX, 50);
//    sct_pwm_setPinDutyCycle(MOTOR2_SCT_CHANNEL, 50);

	// Interrupt sources:
    // ADC
    // UART
    // Timer
    // PININT 1-5
    while (1) {
//    	timer_delay_us(100000);
//    	_U16 adc = adc_readPin(POT_ADC_CHANNEL);
//    	pm_clear_rectangle(10, 10, 120, 20);
//    	char str[20] = "ADC: ";
//    	num2str(adc, str + 5);
//    	pm_place_string(str, 10, 10);
//    	pm_write_buffer();
//    	gpio_togglePinValue(LED_RUNNING_PORT, LED_RUNNING_PIN);
    }

    return 0 ;
}
