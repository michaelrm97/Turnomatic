/*
 * motor.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <compiler.h>

void motor_init(void);

void motor_set(_U32 pos);

void motor_stop(void);

#endif /* MOTOR_H_ */
