/*
 * usb.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <chip.h>

#include <pindefs.h>
#include <project_config.h>

#include <uart.h>
#include <flash.h>

#include <string.h>
#include <pmoleds.h>
#include <pm_graphics.h>
#include <song_table.h>
#include <config_table.h>
#include <adc_handler.h>
#include <user.h>
#include <mode.h>

#include <gpio.h>
#include <adc.h>
#include <motor.h>

// Convert array of bytes (little endian) into an unsigned int
static _U32 bytes2int(_U08 *arr) {
	_U32 ret = 0;
	for (int i = 0; i < 4; i++) {
		ret |= (arr[i] << (i << 3));
	}
	return ret;
}

// Setup uart to interact with ftdi chip
void usb_init(void) {
	uart_rx_pinassign(USB_UART, UART_RX_PORT, UART_RX_PIN);
	uart_tx_pinassign(USB_UART, UART_TX_PORT, UART_TX_PIN);
	uart_cts_pinassign(USB_UART, UART_CTS_PORT, UART_CTS_PIN);
	uart_rts_pinassign(USB_UART, UART_RTS_PORT, UART_RTS_PIN);

	uart_init(USB_UART, UART_BAUDRATE, UART_CFG_DATALEN_8,
			UART_CFG_PARITY_NONE, UART_CFG_STOPLEN_1);

}

void usb_ack(void) {
	// Send a single ack to pc
	uart_sendBytes(USB_UART, (_U08 *) "ACKK", 4);
}

// Callback function for receiving message via uart from ftdi
// Determines type of message then sends appropriate response
void UART0_IRQHandler(void) {
	// Check that we actually have data to receive
//	gpio_togglePinValue(1, 4);
	if (Chip_UART_GetStatus(USB_UART) & UART_STAT_RXRDY) {
		_U08 data[32];
		uart_readBytes(USB_UART, data, 8);
		// Process data
		char command[5];
		strncpy(command, (char *) data, 4);
		command[4] = '\0';
		if (!strcmp(command, "LIST")) {
			_U16 num = song_num();
			data[0] = num & 0xFF;
			data[1] = (num >> 8) & 0xFF;
			_U16 used_pages = song_used_pages();
			data[2] = used_pages & 0xFF;
			data[3] = (used_pages >> 8) & 0xFF;
			data[4] = MAX_SONGS & 0xFF;
			data[5] = (MAX_SONGS >> 8) & 0xFF;
			data[6] = TOTAL_PAGES & 0xFF;
			data[7] = (TOTAL_PAGES >> 8) & 0xFF;

			uart_sendBytes(USB_UART, data, 8);

			for (int i = 0; i < num; i++) {
				uart_sendBytes(USB_UART, (_U08 *) song_name_get(i), MAX_SONG_LEN);
			}

		} else if (!strcmp(command, "ADDS")) {
			_U32 size = bytes2int(&data[4]);
			_U32 available = (TOTAL_PAGES - song_used_pages()) << 8;
			if (size <= available && song_num() < MAX_SONGS) {
				// Return SUCC
				strncpy((char *)data, "SUCC", 4);
				uart_sendBytes(USB_UART, data, 4);
			} else {
				// Return FAIL
				strncpy((char *)data, "FAIL", 4);
				uart_sendBytes(USB_UART, data, 4);
				return;
			}

			// Receive data about song
			uart_readBytes(USB_UART, data, 32);
			user_mode_set(MODE_LOADING);
			user_enter_loading((char *) &data[12], size);
			int i = 0;
			user_update_loading(0);

			_U16 num_chords = bytes2int(&data[0]);
			_U32 addr = song_store(num_chords, (Bar_t *)&data[4], (char *) &data[12]);

			while (i < size) {
				strncpy((char *)data, "ACKK", 4);
				uart_sendBytes(USB_UART, data, 4);
				int j = COPY_SIZE;
				if (size - i < COPY_SIZE) {
					j = size - i;
				}
				uart_readBytes(USB_UART, flash_buffer, j);
				_U32 new_addr = flash_copy(addr, 0, j);
				if (new_addr == addr) {
					strncpy((char *)data, "FAIL", 4);
					uart_sendBytes(USB_UART, data, 4);
					return;
				}
				addr = new_addr;
				i = i + j;
				user_update_loading(i * 100 / size);
			}
			strncpy((char *)data, "SUCC", 4);
			uart_sendBytes(USB_UART, data, 4);

			user_exit_loading();

		} else if (!strcmp(command, "DELS")) {
			_U32 n = bytes2int(&data[4]);
			user_mode_set(MODE_LOADING);
			user_enter_deleting(song_name_get(n));
			if (song_delete(n)) {
				strncpy((char *)data, "SUCC", 4);
			} else {
				strncpy((char *)data, "FAIL", 4);
			}
			uart_sendBytes(USB_UART, data, 4);
			user_exit_loading();
		} else if (!strcmp(command, "POTV")) {
			_U16 val = pos_get_value();
			data[0] = val & 0xFF;
			data[1] = (val >> 8);
			data[2] = 0;
			data[3] = 0;
			uart_sendBytes(USB_UART, data, 4);
		} else if (!strcmp(command, "POTS")) {
			_U32 pos = bytes2int(&data[4]);
			if (pos < 4096) {
				motor_set_pos(pos);
				strncpy((char *)data, "SUCC", 4);
			} else if (((int) pos) == -1) {
				motor_stop();
				strncpy((char *)data, "SUCC", 4);
			}
			else {
				strncpy((char *)data, "FAIL", 4);
			}
			uart_sendBytes(USB_UART, data, 4);
		} else if (!strcmp(command, "CFGV")) {
			uart_sendBytes(USB_UART, (_U08 *)CONFIG_TABLE_BASE, 16);
		} else if (!strcmp(command, "CFGS")) {
			strncpy((char *)data, "SUCC", 4);
			uart_sendBytes(USB_UART, data, 4);
			uart_readBytes(USB_UART, data, 16);
			config_set_values((_U32 *)data);
			strncpy((char *)data, "SUCC", 4);
			uart_sendBytes(USB_UART, data, 4);
		}
	}
}
