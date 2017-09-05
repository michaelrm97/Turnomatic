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

void uart_tx_pinassign(LPC_USART_T *module, _U08 port, _U08 pin);
void uart_rx_pinassign(LPC_USART_T *module, _U08 port, _U08 pin);
void uart_cts_pinassign(LPC_USART_T *module, _U08 port, _U08 pin);
void uart_rts_pinassign(LPC_USART_T *module, _U08 port, _U08 pin);

void uart_init(LPC_USART_T *module, _U32 baudrate,
		_U32 datalen, _U32 parity,	_U32 stopbits);
void uart_deinit(LPC_USART_T *module);

_S32 uart_sendBytes(LPC_USART_T* module, _U08 *data, _S32 num_bytes);
_S32 uart_readBytes(LPC_USART_T* module, _U08 *data, _S32 num_bytes);

#endif /* UART_H_ */
