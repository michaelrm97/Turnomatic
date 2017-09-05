/*
 * gpio.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <gpio.h>
#include <iocon_5410x.h>

void gpio_init(void) {
	/* Enable clocks to all GPIO ports and input mux */
	Chip_GPIO_Init(LPC_GPIO);

	/* Enable pin interrupt register */
	Chip_PININT_Init(LPC_PININT);
}

void gpio_setPortDir(_U08 port, _U32 pinMask, GPIO_DIR direction) {
	Chip_GPIO_SetPortDIR(LPC_GPIO, port, pinMask, direction);
}

void gpio_setPinDir(_U08 port, _U08 pin, GPIO_DIR direction) {
	Chip_GPIO_SetPinDIR(LPC_GPIO, port, pin, direction);
}

void gpio_setPinMode(_U08 port, _U08 pin, GPIO_MODE mode) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
							(IOCON_DIGITAL_EN | (mode << 3)));
}

void gpio_setPinValue(_U08 port, _U08 pin, _U08 setting) {
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, setting);
}

void gpio_togglePinValue(_U08 port, _U08 pin) {
	Chip_GPIO_SetPinToggle(LPC_GPIO, port, pin);
}

_U08 gpio_getValue(_U08 port, _U08 pin) {
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}

void gpio_registerInterruptHandler(_U08 port, _U08 pin,
									_U08 channel,
									GPIO_INT_MODE mode,
									GPIO_INT_EDGE edge,
									GPIO_INT_LEVEL level,
									void(*handler)(void)) {

}

void gpio_clearInterruptHandler(_U08 channel) {

}

void gpio_enableInterrupt(_U08 channel) {

}

void gpio_disableInterrupt(_U08 channel) {

}
