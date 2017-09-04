/*
 * spi.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <spi.h>

void spi_init(LPC_SPI_T* module, _U32 bitrate,
				SPI_CLOCK_MODE_T clock_mode,
				SPI_DATA_ORDER data_order) {

}

void spi_deinit(LPC_SPI_T* module) {

}

void spi_mosi_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin) {

}

void spi_miso_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin) {

}

void spi_sck_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin) {

}

void spi_cs_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin) {

}

_S32 spi_sendBytes(LPC_SPI_T* module, _U08 *data, _U08 num_bytes, _U08 cs) {
	return 0;
}

_S32 spi_receiveBytes(LPC_SPI_T* module, _U08 *data, _U08 num_bytes, _U08 cs) {
	return 0;
}

_S32 spi_sendReceiveBytes(LPC_SPI_T* module, _U08 *tx_data, _U08 *rx_data, _U08 num_bytes, _U08 cs) {
	return 0;
}
