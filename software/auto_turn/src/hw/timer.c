/*
 * timer.c
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#include <timer.h>

#include <timer_5410x.h>

// Initialise timer
void timer_init(void) {
	Chip_TIMER_Init(DELAY_TIMER); // Timer 0 for delay
	Chip_TIMER_Init(PERIOD_TIMER); // Timer 1 for periodic

	_U08 prescale = Chip_Clock_GetSystemClockRate()/1000000 - 1;
	Chip_TIMER_PrescaleSet(DELAY_TIMER, prescale);
	Chip_TIMER_PrescaleSet(PERIOD_TIMER, prescale);
}

// Delay for given number of us
void timer_delay_us(_U32 delay) {
	Chip_TIMER_ClearMatch(DELAY_TIMER, 0);
	Chip_TIMER_Reset(DELAY_TIMER);
	Chip_TIMER_ResetOnMatchEnable(DELAY_TIMER, 0);
	Chip_TIMER_StopOnMatchEnable(DELAY_TIMER, 0);
	Chip_TIMER_SetMatch(DELAY_TIMER, 0, delay);
	Chip_TIMER_Enable(DELAY_TIMER);
	while (Chip_TIMER_ReadCount(DELAY_TIMER) < delay);
}

// Set timer to generate a regular interrupt
// Period given in us
void timer_set_periodic(_U32 period) {
	Chip_TIMER_ClearMatch(PERIOD_TIMER, 0);
	Chip_TIMER_Reset(PERIOD_TIMER);
	Chip_TIMER_ResetOnMatchEnable(PERIOD_TIMER, 0);
	Chip_TIMER_MatchEnableInt(PERIOD_TIMER, 0);
	Chip_TIMER_MatchDisableInt(PERIOD_TIMER, 1);
	Chip_TIMER_SetMatch(PERIOD_TIMER, 0, period);
	NVIC_ClearPendingIRQ(PERIOD_TIMER_NVIC);
	NVIC_EnableIRQ(PERIOD_TIMER_NVIC);
	Chip_TIMER_Enable(PERIOD_TIMER);
}

// Unset periodic interrupt generation
void timer_unset_periodic(void) {
	Chip_TIMER_Disable(LPC_TIMER1);
	NVIC_ClearPendingIRQ(CT32B1_IRQn);
	NVIC_DisableIRQ(CT32B1_IRQn);
}
