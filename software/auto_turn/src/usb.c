/*
 * usb.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <pindefs.h>
#include <project_config.h>

#include <uart.h>

void usb_init(void) {
	uart_rx_pinassign(USB_UART, UART_RX_PORT, UART_RX_PIN);
	uart_tx_pinassign(USB_UART, UART_TX_PORT, UART_TX_PIN);
	uart_cts_pinassign(USB_UART, UART_CTS_PORT, UART_CTS_PIN);
	uart_rts_pinassign(USB_UART, UART_RTS_PORT, UART_RTS_PIN);

	uart_init(USB_UART, UART_BAUDRATE, UART_CFG_DATALEN_8,
			UART_CFG_PARITY_NONE, UART_CFG_STOPLEN_1);

}

void UART0_IRQHandler(void) {

}
