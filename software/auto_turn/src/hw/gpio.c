/*
 * gpio.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <gpio.h>

#include <gpio_5410x.h>
#include <iocon_5410x.h>
#include <pinint_5410x.h>

// Initialise gpio module
void gpio_init(void) {
	/* Enable clocks to all GPIO ports and input mux */
	Chip_GPIO_Init(LPC_GPIO);

	/* Enable pin interrupt register */
	Chip_PININT_Init(LPC_PININT);
}

// Set direction for an entire port
void gpio_setPortDir(_U08 port, _U32 pinMask, GPIO_DIR direction) {
	Chip_GPIO_SetPortDIR(LPC_GPIO, port, pinMask, direction);
}

// Set direction for a single pin
void gpio_setPinDir(_U08 port, _U08 pin, GPIO_DIR direction) {
	Chip_GPIO_SetPinDIR(LPC_GPIO, port, pin, direction);
}

// Set mode for a given pin
void gpio_setPinMode(_U08 port, _U08 pin, GPIO_MODE mode) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
							(IOCON_DIGITAL_EN | (mode << 3)));
}

// Set value for a given pin
void gpio_setPinValue(_U08 port, _U08 pin, _U08 setting) {
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, setting);
}

// Toggle pin value
void gpio_togglePinValue(_U08 port, _U08 pin) {
	Chip_GPIO_SetPinToggle(LPC_GPIO, port, pin);
}

// Get value for a given pin
_U08 gpio_getValue(_U08 port, _U08 pin) {
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}

// Assign an interrupt for a particular pin and pinint channel
// Takes in GPIO mode, and interrupt settings
void gpio_assignInterrupt(_U08 port, _U08 pin,
							_U08 channel,
							GPIO_MODE mode,
							GPIO_INT_MODE imode,
							GPIO_INT_EDGE edge,
							GPIO_INT_LEVEL level) {

	/* Set pin to GPIO */
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_DIGITAL_EN | (mode << 3)));

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);

	/* Set pin to corresponding pin interrupt vector */
	Chip_INMUX_PinIntSel(channel, port, pin);
	/* Clear interrupt status just in case */
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(channel));

	/* Set interrupt detection type */
	if (imode == GPIO_INT_MODE_EDGE) {
		Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(channel));
		if (edge == GPIO_INT_EDGE_RISING || edge == GPIO_INT_EDGE_ALL) {
			Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(channel));
		}
		if (edge == GPIO_INT_EDGE_FALLING || edge == GPIO_INT_EDGE_ALL) {
			Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(channel));
		}
	}
	else if (imode == GPIO_INT_MODE_LEVEL) {
		Chip_PININT_SetPinModeLevel(LPC_PININT, PININTCH(channel));
		if (level == GPIO_INT_LEVEL_LOW) {
			Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(channel));
		}
		else if (level == GPIO_INT_LEVEL_HIGH) {
			Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(channel));
		}
	}

}

// Enable/ disable interrupt at a given pin
void gpio_enableInterrupt(_U08 channel, bool enable) {
	if (enable) {
		Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(channel));
		if (channel < 4) {
			NVIC_ClearPendingIRQ(PIN_INT0_IRQn + channel);
			NVIC_EnableIRQ(PIN_INT0_IRQn + channel);
		} else if (channel < 8) {
			NVIC_ClearPendingIRQ(PIN_INT4_IRQn + channel - 4);
			NVIC_EnableIRQ(PIN_INT4_IRQn + channel - 4);
		}
	} else {
		if (channel < 4) {
			NVIC_DisableIRQ(PIN_INT0_IRQn + channel);
		} else if (channel < 8) {
			NVIC_DisableIRQ(PIN_INT4_IRQn + channel - 4);
		}
	}
}
