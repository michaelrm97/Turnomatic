/*
 * spi.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef SPI_H_
#define SPI_H_

#include <chip.h>
#include <compiler.h>

#include <spi_common_5410x.h>

typedef enum SPI_DATA_ORDER {
	SPI_DATA_MSB_FIRST = 0,
	SPI_DATA_LSB_FIRST = 1
} SPI_DATA_ORDER;

// Setup pins for spi
void spi_mosi_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin);
void spi_miso_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin);
void spi_sck_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin);
void spi_cs_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin);

// Initialise/ deinitialise spi module
void spi_init(LPC_SPI_T* module, _U32 bitrate,
				SPI_CLOCK_MODE_T clock_mode,
				SPI_DATA_ORDER data_order);
void spi_deinit(LPC_SPI_T* module);

// Send/ receive data through spi
void spi_sendBytes(LPC_SPI_T* module, _U08 *data, _U08 num_bytes);
void spi_receiveBytes(LPC_SPI_T* module, _U08 *data, _U08 num_bytes);
void spi_sendReceiveBytes(LPC_SPI_T* module, _U08 *tx_data, _U08 *rx_data, _U08 num_tx, _U08 num_rx);

#endif /* SPI_H_ */
