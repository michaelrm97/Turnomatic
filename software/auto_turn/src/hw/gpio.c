/*
 * gpio.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <gpio.h>

void gpio_init(void) {
	/* Enable clocks to all GPIO ports and input mux */
	Chip_GPIO_Init(LPC_GPIO);

	/* Enable pin interrupt register */
	Chip_PININT_Init(LPC_PININT);
}

void gpio_setPortDir(_U08 port, _U32 pinMask, GPIO_DIR direction) {

}

void gpio_setPinDir(_U08 port, _U08 pin, GPIO_DIR direction) {

}

void gpio_setPinMode(_U08 port, _U08 pin, GPIO_MODE mode) {

}

void gpio_setPinValue(_U08 port, _U08 pin, _U08 setting) {

}

void gpio_togglePinValue(_U08 port, _U08 pin) {

}

_U08 gpio_getValue(_U08 port, _U08 pin) {
	return 0;
}

void gpio_clearInterruptHandler(_U08 port, _U08 pin) {

}

void gpio_enableInterrupt(_U08 port, _U08 pin) {

}

void gpio_disableInterrupt(_U08 port, _U08 pin) {

}
