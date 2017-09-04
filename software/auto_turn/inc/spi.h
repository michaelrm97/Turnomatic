/*
 * spi.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef SPI_H_
#define SPI_H_

#include <compiler.h>

#include <chip.h>
#include <spi_common_5410x.h>
#include <spim_5410x.h>

typedef enum SPI_CLOCK_MODE {
	SPI_CLOCK_CPHA0_CPOL0 	= 0,
	SPI_CLOCK_CPHA0_CPOL1 	= 1,
	SPI_CLOCK_CPHA1_CPOL0	= 2,
	SPI_CLOCK_CPHA1_CPOL1 	= 3,
	SPI_CLOCK_MODE0 			= 0,	// Alias for CPHA=0, CPOL=0
	SPI_CLOCK_MODE1 			= 1,	// Alias for CPHA=0, CPOL=1
	SPI_CLOCK_MODE2 			= 2,	// Alias for CPHA=1, CPOL=0
	SPI_CLOCK_MODE3 			= 3		// Alias for CPHA=1, CPOL=1
} SPI_CLOCK_MODE;

typedef enum SPI_DATA_ORDER {
	SPI_DATA_MSB_FIRST = 0,
	SPI_DATA_LSB_FIRST = 1
} SPI_DATA_ORDER;

void spi_init(LPC_SPI_T* module, _U32 bitrate,
				SPI_CLOCK_MODE clock_mode,
				SPI_DATA_ORDER data_order);

void spi_deinit(LPC_SPI_T* module);

void spi_mosi_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin);
void spi_miso_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin);
void spi_sck_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin);

_S32 spi_sendBytes(LPC_SPI_T* module, _U08 *data, _U08 num_bytes);
_S32 spi_receiveBytes(LPC_SPI_T* module, _U08 *data, _U08 num_bytes);
_S32 spi_sendReceiveBytes(LPC_SPI_T* module, _U08 *tx_data, _U08 *rx_data, _U08 num_bytes);



#endif /* SPI_H_ */
