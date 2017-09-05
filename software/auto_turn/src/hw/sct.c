/*
 * sct.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <sct.h>

#include <sct_5410x.h>
#include <sct_pwm_5410x.h>

void sct_init(void) {
	Chip_SCT_Init(LPC_SCT0);
	LPC_SCT0->CONFIG = SCT_CONFIG_16BIT_COUNTER | SCT_CONFIG_CLKMODE_PRESCALED_SYSCLK |
			SCT_CONFIG_CKSEL_RISING_IN_0 | SCT_CONFIG_NORELOADL_U | SCT_CONFIG_NORELOADH |
			SCT_CONFIG_AUTOLIMIT_L | SCT_CONFIG_AUTOLIMIT_H;
	_U08 prescale_l = Chip_Clock_GetSystemClockRate()/2000000; // 0.5us ticks
	_U08 prescale_h = Chip_Clock_GetSystemClockRate()/1000000; // 1us ticks
	LPC_SCT0->CTRL_L = SCT_CTRL_BIDIR_L(0) | SCT_CTRL_PRE_L(prescale_l) | SCT_CTRL_STOP_L;
	LPC_SCT0->CTRL_H = SCT_CTRL_BIDIR_H(0) | SCT_CTRL_PRE_H(prescale_h) | SCT_CTRL_STOP_H;

	// Event 0 AND 1 for ADC
	LPC_SCT0->EVENT[0].STATE = 0xFFFF;
	LPC_SCT0->EVENT[0].CTRL = SCT_EV_CTRL_MATCHSEL(0) | SCT_EV_CTRL_HEVENT_L |
			SCT_EV_CTRL_COMBMODE_MATCH | SCT_EV_CTRL_DIRECTION_INDEPENDENT;

	LPC_SCT0->MATCH[1].L = 0;
	LPC_SCT0->EVENT[1].STATE = 0xFFFF;
	LPC_SCT0->EVENT[1].CTRL = SCT_EV_CTRL_MATCHSEL(1) | SCT_EV_CTRL_HEVENT_L |
			SCT_EV_CTRL_COMBMODE_MATCH | SCT_EV_CTRL_DIRECTION_INDEPENDENT;

	// Clear at timer = 0, set at timer = period - 1
	LPC_SCT->OUT[7].SET = (1<<0);
	LPC_SCT->OUT[7].CLR = (1<<1);

	// Event 2 for ADC
	LPC_SCT0->EVENT[2].STATE = 0xFFFF;
	LPC_SCT0->EVENT[2].CTRL = SCT_EV_CTRL_MATCHSEL(0) | SCT_EV_CTRL_HEVENT_H |
			SCT_EV_CTRL_COMBMODE_MATCH | SCT_EV_CTRL_DIRECTION_INDEPENDENT;

}

// Period in 0.5us
void sct_set_period(_U32 period) {
	LPC_SCT0->MATCH[0].L = period - 1;
	LPC_SCT0->CTRL_L |= SCT_CTRL_CLRCTR_L;
	LPC_SCT0->CTRL_L &= ~SCT_CTRL_STOP_L;
}

void sct_unset_period(void) {
	LPC_SCT0->CTRL_L |= SCT_CTRL_STOP_L;
}

void sct_pwm_init(_U32 frequency);
void sct_pwm_pinassign(_U08 port, _U08 pin, _U08 channel);
void sct_pwm_setPinDutyCycle(_U08 channel, float duty);

