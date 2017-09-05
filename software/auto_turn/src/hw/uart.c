/*
 * uart.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <uart.h>

void uart_tx_pinassign(LPC_USART_T *module, _U08 port, _U08 pin) {
	int i = port * 32 + pin;
	if (module == LPC_USART0) {
		switch(i) {
		case 1:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 21:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	} else if (module == LPC_USART1) {
		switch(i) {
		case 6:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 12:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 42:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	} else	if (module == LPC_USART2) {
		switch(i) {
		case 9:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		}
	} else	if (module == LPC_USART3) {
		switch(i) {
		case 45:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	}
}

void uart_rx_pinassign(LPC_USART_T *module, _U08 port, _U08 pin) {
	int i = port * 32 + pin;
	if (module == LPC_USART0) {
		switch(i) {
		case 0:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 22:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	} else if (module == LPC_USART1) {
		switch(i) {
		case 5:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 11:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	} else	if (module == LPC_USART2) {
		switch(i) {
		case 8:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 14:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	} else	if (module == LPC_USART3) {
		switch(i) {
		case 20:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 44:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	}
}

void uart_cts_pinassign(LPC_USART_T *module, _U08 port, _U08 pin) {
	int i = port * 32 + pin;
	if (module == LPC_USART0) {
		switch(i) {
		case 2:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		}
	} else if (module == LPC_USART1) {
		switch(i) {
		case 16:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 25:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	} else	if (module == LPC_USART2) {
		switch(i) {
		case 31:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	}
}

void uart_rts_pinassign(LPC_USART_T *module, _U08 port, _U08 pin) {
	int i = port * 32 + pin;
	if (module == LPC_USART0) {
		switch(i) {
		case 3:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		}
	} else if (module == LPC_USART1) {
		switch(i) {
		case 17:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 43:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	} else	if (module == LPC_USART2) {
		switch(i) {
		case 32:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	}
}

void uart_init(LPC_USART_T *module, _U32 baudrate,
		_U32 datalen, _U32 parity,	_U32 stopbits) {

	Chip_Clock_SetUARTBaseClockRate(Chip_Clock_GetMainClockRate());

	Chip_UART_Init(module);
	_U32 cfg = datalen | parity | stopbits;

	Chip_UART_ConfigData(module, cfg);
	Chip_UART_SetBaud(module, baudrate);
	Chip_UART_Enable(module);
	Chip_UART_TXEnable(module);

}

void uart_deinit(LPC_USART_T *module) {
	Chip_UART_DeInit(module);
}

_S32 uart_sendBytes(LPC_USART_T* module, _U08 *data, _S32 num_bytes) {
	return Chip_UART_SendBlocking(module, data, num_bytes);
}

_S32 uart_readBytes(LPC_USART_T* module, _U08 *data, _S32 num_bytes) {
	return Chip_UART_ReadBlocking(module, data, num_bytes);
}
