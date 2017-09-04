/*
 * gpio.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <gpio.h>

void gpio_init(void);

void gpio_setPortDir(_U08 port, _U32 pinMask, GPIO_DIR direction);
void gpio_setPinDif(_U08 port, _U08 pin, GPIO_DIR direction);
void gpio_setPinMode(_U08 port, _U08 pin, GPIO_MODE mode);
void gpio_setPinValue(_U08 port, _U08 pin, _U08 setting);
void gpio_togglePinValue(_U08 port, _U08 pin);

_U08 gpio_getValue(_U08 port, _U08 pin);

void gpio_clearInterruptHandler(_U08 port, _U08 pin);
void gpio_enableInterrupt(_U08 port, _U08 pin);
void gpio_disableInterrupt(_U08 port, _U08 pin);
