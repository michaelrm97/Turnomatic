/*
 * motor.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <compiler.h>

#include <song.h>

void motor_init(void);

void motor_set_page(Page_t page);
void motor_set_pos(_U32 pos);

void motor_stop(void);

#endif /* MOTOR_H_ */
