/*
 * motor.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <pindefs.h>
#include <project_config.h>

#include <motor.h>

#include <timer.h>
#include <adc.h>
#include <sct.h>
#include <gpio.h>

_U32 motor_pos;

void motor_init(void) {

	// Assign pins
	gpio_setPinDir(BUCK_EN_PORT, BUCK_EN_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(BUCK_EN_PORT, BUCK_EN_PIN, GPIO_MODE_NONE);
	gpio_setPinValue(BUCK_EN_PORT, BUCK_EN_PIN, 0);

	gpio_setPinDir(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, GPIO_MODE_NONE);
	gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 0);

	gpio_setPinDir(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, GPIO_MODE_NONE);
	gpio_setPinValue(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, 0);

	Chip_IOCON_PinMuxSet(LPC_IOCON, POT_ADC_PORT, POT_ADC_PIN, IOCON_INPFILT_OFF);
}

void motor_set(_U32 pos) {
	motor_pos = pos;
	gpio_setPinValue(BUCK_EN_PORT, BUCK_EN_PIN, 1); // Turn on buck
	// Set timer to sample pot and adjust motor periodically
	timer_set_periodic(MOTOR_SAMPLE_PERIOD);
}

void motor_stop(void) {
	// Stop motor
	gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 0);
	gpio_setPinValue(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, 0);
	gpio_setPinValue(BUCK_EN_PORT, BUCK_EN_PIN, 0); // Turn off buck
	timer_unset_periodic(); // Stop sampling
}

void CT32B2_IRQHandler(void) {
	// Rudimentary control system
	// In future will take derivative into account
	Chip_TIMER_ClearMatch(PERIOD_TIMER, 0);
	_U32 curr_pos = adc_readPin(POT_ADC_PIN);
	if (curr_pos - motor_pos < MOTOR_THRESHOLD || motor_pos - curr_pos < MOTOR_THRESHOLD) {
		motor_stop();
	} else if (curr_pos < motor_pos) {
		// Go forwards
		gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 1);
		gpio_setPinValue(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, 0);
	} else {
		gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 0);
		gpio_setPinValue(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, 1);
	}
}
