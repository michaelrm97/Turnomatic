/*
 * motor.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <pindefs.h>
#include <project_config.h>

#include <motor.h>

#include <adc.h>
#include <gpio.h>
#include <timer.h>

#include <config_table.h>

// Target motor position
static _U32 motor_pos;

// Initialise motor
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

	adc_pinassign(POT_ADC_PORT, POT_ADC_PIN);
}

// Make the motor target a particular page
void motor_set_page(Page_t page) {
	_U32 page_pos = MOTOR_PAGE1_POS + (page - 1) * MOTOR_PAGE_POS_DIFF;
	motor_set_pos(page_pos);
}

// Make the motor target a particular position (pot value)
void motor_set_pos(_U32 pos) {
	motor_pos = pos;
	gpio_setPinValue(BUCK_EN_PORT, BUCK_EN_PIN, 1); // Turn on buck
	// Set timer to sample pot and adjust motor periodically
	timer_set_periodic(MOTOR_SAMPLE_PERIOD);
}

// Stop the motor
void motor_stop(void) {
	// Stop motor
	gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 0);
	gpio_setPinValue(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, 0);
	gpio_setPinValue(BUCK_EN_PORT, BUCK_EN_PIN, 0); // Turn off buck
	timer_unset_periodic(); // Stop sampling
}

// Motor interrupt handler
// On first interrupt, pot value is read and motor pwm duty cycle is determined
// Motor is set to turn in one direction and timer match register is set
// To generate another interrupt which turns the motor off
void CT32B2_IRQHandler(void) {
	// Rudimentary control system
	// In future will take derivative into account
	if (Chip_TIMER_MatchPending(PERIOD_TIMER, 0)) {
		Chip_TIMER_ClearMatch(PERIOD_TIMER, 0);
		_U16 ticks;
		_U32 curr_pos = adc_readPin(POT_ADC_CHANNEL);
		if (curr_pos - motor_pos < MOTOR_THRESHOLD || motor_pos - curr_pos < MOTOR_THRESHOLD) {
			motor_stop();
		} else if (curr_pos < motor_pos) {
			// Work out duty cycle
			ticks = (motor_pos - curr_pos) * MOTOR_GAIN; // How long to keep motor on in ticks
			// Go forwards
			gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 1);
			gpio_setPinValue(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, 0);
			if (ticks < MOTOR_SAMPLE_PERIOD) { // Disable motor after certain time
				Chip_TIMER_SetMatch(PERIOD_TIMER, 1, ticks);
				Chip_TIMER_MatchEnableInt(PERIOD_TIMER, 1);
			} else {
				Chip_TIMER_MatchDisableInt(PERIOD_TIMER, 1);
			}
		} else {
			// Work out duty cycle
			ticks = (curr_pos - motor_pos) * MOTOR_GAIN; // How long to keep motor on in ticks
			gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 0);
			gpio_setPinValue(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, 1);
			if (ticks < MOTOR_SAMPLE_PERIOD) {
				Chip_TIMER_SetMatch(PERIOD_TIMER, 1, ticks);
				Chip_TIMER_MatchEnableInt(PERIOD_TIMER, 1);
			} else {
				Chip_TIMER_MatchDisableInt(PERIOD_TIMER, 1);
			}
		}
	}
	if (Chip_TIMER_MatchPending(PERIOD_TIMER, 1)) {
		Chip_TIMER_ClearMatch(PERIOD_TIMER, 1);
		gpio_setPinValue(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, 1);
		gpio_setPinValue(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, 1);
	}
}
