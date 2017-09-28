/*
 * timer.h
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chip.h>
#include <compiler.h>

#define DELAY_TIMER LPC_TIMER3 // Timer to implement delays with
#define PERIOD_TIMER LPC_TIMER2 // Timer to implement periodic function
#define PERIOD_TIMER_NVIC CT32B2_IRQn

// Initialise timer
void timer_init(void);

// Delay for given number of us
void timer_delay_us(_U32 delay);

// Set/ unset timer to generate a regular interrupt
// Period given in us
void timer_set_periodic(_U32 period);
void timer_unset_periodic(void);

#endif /* TIMER_H_ */
