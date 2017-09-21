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

static void motor_set_speed(float speed) {
	float pwm1, pwm2;
	if (speed >= 0) {
		pwm1 = 100;
		pwm2 = 100 - speed;
		if (pwm2 < 0) {
			pwm2 = 0;
		}
		if (pwm2 > 100) {
			pwm2 = 100;
		}
	} else {
		pwm2 = 100;
		pwm1 = 100 + speed;
		if (pwm1 < 0) {
			pwm1 = 0;
		}
		if (pwm1 > 100) {
			pwm1 = 100;
		}
	}
	sct_pwm_setPinDutyCycle(MOTOR1_SCT_CHANNEL, pwm1);
	sct_pwm_setPinDutyCycle(MOTOR2_SCT_CHANNEL, pwm2);
}

void motor_init(void) {

	gpio_setPinDir(BUCK_EN_PORT, BUCK_EN_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(BUCK_EN_PORT, BUCK_EN_PIN, GPIO_MODE_NONE);
	gpio_setPinValue(BUCK_EN_PORT, BUCK_EN_PIN, 0);

	// Assign pins
    sct_pwm_init(1000);
//	sct_pwm_pinassign(MOTOR_IN_1_PORT, MOTOR_IN_1_PIN, MOTOR1_SCT_CHANNEL);
    Chip_IOCON_PinMuxSet(LPC_IOCON, MOTOR_IN_1_PORT, MOTOR_IN_1_PIN,
    		IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2);
//	sct_pwm_pinassign(MOTOR_IN_2_PORT, MOTOR_IN_2_PIN, MOTOR2_SCT_CHANNEL);
	sct_pwm_setOutPin(MOTOR1_SCT_INDEX, MOTOR1_SCT_CHANNEL);
//	sct_pwm_setOutPin(MOTOR2_SCT_INDEX, MOTOR2_SCT_CHANNEL);
	sct_pwm_setPinDutyCycle(MOTOR1_SCT_INDEX, 0);
	sct_pwm_start();
	Chip_IOCON_PinMuxSet(LPC_IOCON, POT_ADC_PORT, POT_ADC_PIN, IOCON_INPFILT_OFF);
}

void motor_set(_U32 pos) {
	motor_pos = pos;

	// Init PWM module
	sct_pwm_init(PWM_FREQUENCY);
	// Set PWM duty cycles as 0
	sct_pwm_setPinDutyCycle(MOTOR1_SCT_CHANNEL, 0);
	sct_pwm_setPinDutyCycle(MOTOR2_SCT_CHANNEL, 0);
	// Set timer to sample pot and adjust motor periodically
	timer_set_periodic(MOTOR_SAMPLE_PERIOD);
}

void motor_stop(void) {
	// Set PWM duty cycles as 0
	sct_pwm_stop();

	timer_unset_periodic();
}

void CT32B2_IRQHandler(void) {
	Chip_TIMER_ClearMatch(PERIOD_TIMER, 0);
	_U32 curr_pos = adc_readPin(POT_ADC_PIN);
	if (curr_pos - motor_pos < MOTOR_THRESHOLD || motor_pos - curr_pos < MOTOR_THRESHOLD) {
		motor_stop();
	} else {
		motor_set_speed(MOTOR_GAIN * (motor_pos - curr_pos));
	}
}
