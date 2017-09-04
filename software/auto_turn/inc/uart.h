/*
 * uart.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef UART_H_
#define UART_H_

#include <compiler.h>

#include <chip.h>
#include <uart_5410x.h>

typedef enum UART_DATALEN {
	UART_DATALEN_7 = 0,
	UART_DATALEN_8,
	UART_DATALEN_9
} UART_DATALEN;

typedef enum UART_PARITY {
	UART_PARITY_NONE = 0,
	UART_PARITY_EVEN,
	UART_PARITY_ODD
} UART_PARITY;

typedef enum UART_STOPLEN {
	UART_STOPLEN_1 = 0,
	UART_STOPLEN_2
} UART_STOPLEN;

void uart_tx_pinassign(LPC_USART_T *module, _U08 port, _U08 pin);
void uart_rx_pinassign(LPC_USART_T *module, _U08 port, _U08 pin);

void uart_init(LPC_USART_T *module, _U32 baudrate,
				UART_DATALEN datalen,
				UART_PARITY parity,
				UART_STEPLEN stopbits);
void uart_deinit(LPC_USART_T *module);

_S32 tc_uart_sendBytes(LPC_USART_T* module, _U08 *data, _S32 num_bytes);
_S32 tc_uart_readBytes(LPC_USART_T* module, _U08 *data, _S32 num_bytes);

#endif /* UART_H_ */
