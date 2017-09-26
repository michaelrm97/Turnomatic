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
#include <user.h>
#include <mode.h>

static _U32 bytes2int(_U08 *arr) {
	_U32 ret = 0;
	for (int i = 0; i < 4; i++) {
		ret |= (arr[i] << (i << 3));
	}
	return ret;
}

static _U16 bytes2short(_U08 *arr) {
	_U16 ret = 0;
	for (int i = 0; i < 2; i++) {
		ret |= (arr[i] << (i << 3));
	}
	return ret;
}

void usb_init(void) {
	uart_rx_pinassign(USB_UART, UART_RX_PORT, UART_RX_PIN);
	uart_tx_pinassign(USB_UART, UART_TX_PORT, UART_TX_PIN);
	uart_cts_pinassign(USB_UART, UART_CTS_PORT, UART_CTS_PIN);
	uart_rts_pinassign(USB_UART, UART_RTS_PORT, UART_RTS_PIN);

	uart_init(USB_UART, UART_BAUDRATE, UART_CFG_DATALEN_8,
			UART_CFG_PARITY_NONE, UART_CFG_STOPLEN_1);

}

void UART0_IRQHandler(void) {
	// Check that we actually have data to receive
	if (Chip_UART_GetStatus(USB_UART) & UART_STAT_RXRDY) {
		_U08 data[32];
		_U16 used_pages;
		_U16 free_pages;
		uart_readBytes(USB_UART, data, 8);
		// Process data
		char command[5];
		strncpy(command, (char *) data, 4);
		command[4] = '\0';
		if (!strcmp(command, "LIST")) {
			_U16 num = song_num() & 0xFFFF;
			data[0] = num & 0xFF;
			data[1] = (num >> 8) & 0xFF;
			flash_pages(&used_pages, &free_pages);
			_U16 total_pages = used_pages + free_pages;
			data[2] = total_pages & 0xFF;
			data[3] = (total_pages >> 8) & 0xFF;
			data[4] = used_pages & 0xFF;
			data[5] = (used_pages >> 8) & 0xFF;
			data[6] = free_pages & 0xFF;
			data[7] = (free_pages >> 8) & 0xFF;

			uart_sendBytes(USB_UART, data, 8);

			for (int i = 0; i < num; i++) {
				_U08 *name = (_U08 *) song_name_get(i);
				uart_sendBytes(USB_UART, name, MAX_SONG_LEN);
			}

		} else if (!strcmp(command, "ADDS")) {
			_U32 size = bytes2int(&data[4]);
			flash_pages(&used_pages, &free_pages);
			if (size <= (free_pages >> 8)) {
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

			_U16 num_chords = bytes2short(&data[0]);
			_U32 addr = song_store(num_chords, (Bar_t *)&data[4], (char *) &data[8]);
			user_mode_set(MODE_LOADING);
			user_enter_loading((char *) &data[8], size);
			int i = 0;
			user_update_loading(i, size);
			while (i < size) {
				int j = BUFFER_SIZE;
				if (size - i < BUFFER_SIZE) {
					j = size - i;
				}
				uart_readBytes(USB_UART, flash_buffer, j);
				_U32 new_addr = flash_copy(addr, j);
				if (new_addr != addr) {
					addr = new_addr;
				} else {
					strncpy((char *)data, "FAIL", 4);
					uart_sendBytes(USB_UART, data, 4);
					return;
				}
				user_update_loading(i, size);
			}
			strncpy((char *)data, "SUCC", 4);
			uart_sendBytes(USB_UART, data, 4);

			user_exit_loading();

		} else if (!strcmp(command, "DELS")) {
			_U32 n = bytes2int(&data[4]);
			user_enter_deleting(song_name_get(n));
			if (song_delete(n)) {
				strncpy((char *)data, "SUCC", 4);
				uart_sendBytes(USB_UART, data, 4);
			} else {
				strncpy((char *)data, "FAIL", 4);
				uart_sendBytes(USB_UART, data, 4);
			}
			user_update_deleting(100);
			user_exit_loading();
		}
	}
}
