/*
 * motor.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <chip.h>
#include <compiler.h>

#include <song.h>

// Initialise motor
void motor_init(void);

// Make the motor target a particular page or position (pot value)
void motor_set_page(Page_t page);
void motor_set_pos(_U32 pos);

// Stop the motor
void motor_stop(void);

#endif /* MOTOR_H_ */
