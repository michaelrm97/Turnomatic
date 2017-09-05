/*
 * timer.h
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <compiler.h>

#include <chip.h>

#define DELAY_TIMER LPC_TIMER0 // Timer to implement delays with
#define PERIOD_TIMER LPC_TIMER1 // Timer to implement periodic function
#define PERIOD_TIMER_NVIC CT32B1_IRQn

void timer_init(void);

void timer_delay_us(_U32 delay);

void timer_set_periodic(_U32 period);
void timer_unset_periodic(void);

#endif /* TIMER_H_ */
