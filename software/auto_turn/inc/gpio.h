/*
 * gpio.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <compiler.h>

#include <chip.h>
#include <iocon_5410x.h>
#include <gpio_5410x.h>
#include <pinint_5410x.h>

typedef enum GPIO_DIR {
	GPIO_DIR_INPUT = 0,
	GPIO_DIR_OUTPUT
} GPIO_DIR;

typedef enum GPIO_MODE {
	GPIO_MODE_NONE = 0,
	GPIO_MODE_PULLDOWN,
	GPIO_MODE_PULLUP,
	GPIO_MODE_REPEATER
} GPIO_MODE;

typedef enum GPIO_INT_MODE
{
	GPIO_INT_MODE_EDGE = 0,
	GPIO_INT_MODE_LEVEL
} GPIO_INT_MODE;

typedef enum GPIO_INT_EDGE {
	GPIO_INT_EDGE_RISING = 0,
	GPIO_INT_EDGE_FALLING,
	GPIO_INT_EDGE_ALL
} GPIO_INT_EDGE;

typedef enum GPIO_INT_LEVEL {
	GPIO_INT_LEVEL_LOW = 0,
	GPIO_INT_LEVEL_HIGH
} GPIO_INT_LEVEL;

void gpio_init(void);
void gpio_setPortDir(_U08 port, _U32 pinMask, GPIO_DIR direction);
void gpio_setPinDir(_U08 port, _U08 pin, GPIO_DIR direction);
void gpio_setPinMode(_U08 port, _U08 pin, GPIO_MODE mode);
void gpio_setPinValue(_U08 port, _U08 pin, _U08 setting);
void gpio_togglePinValue(_U08 port, _U08 pin);

_U08 gpio_getValue(_U08 port, _U08 pin);

void gpio_assignInterrupt(_U08 port, _U08 pin,
							_U08 channel,
							GPIO_MODE mode,
							GPIO_INT_MODE imode,
							GPIO_INT_EDGE edge,
							GPIO_INT_LEVEL level);
void gpio_enableInterrupt(_U08 channel);
void gpio_disableInterrupt(_U08 channel);

#endif /* GPIO_H_ */
