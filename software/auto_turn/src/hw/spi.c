/*
 * spi.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <spi.h>

#include <spim_5410x.h>

// Setup pin for mosi
void spi_mosi_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin) {
	_U08 i = port * 32 + pin;
	if (module == LPC_SPI0) {
		switch(i) {
		case 12:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 41:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
	} else if (module == LPC_SPI1) {
		switch(i) {
		case 39:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 45:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC4));
			break;
		}
	}
}

// Setup pin for miso
void spi_miso_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin) {
	_U08 i = port * 32 + pin;
	if (module == LPC_SPI0) {
		switch(i) {
		case 13:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 36:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC5));
			break;
		}
	} else if (module == LPC_SPI1) {
		switch(i) {
		case 40:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 46:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC5));
			break;
		}
	}
}

// Setup pin for sck
void spi_sck_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin) {
	_U08 i = port * 32 + pin;
	if (module == LPC_SPI0) {
		switch(i) {
		case 11:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 35:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC5));
			break;
		}
	} else if (module == LPC_SPI1) {
		switch(i) {
		case 38:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 44:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC4));
			break;
		}
	}
}

// Setup pin for cs (note- not used- gpio used instead)
void spi_cs_pinassign(LPC_SPI_T* module, _U08 port, _U08 pin) {
	_U08 i = port * 32 + pin;
	if (module == LPC_SPI0) {
		switch(i) {
		case 0:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 1:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 4:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 9:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC5));
			break;
		case 14:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 15:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 16:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		case 17:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC1));
			break;
		}
	} else if (module == LPC_SPI1) {
		switch(i) {
		case 34:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 35:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 36:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 37:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2	));
			break;
		case 47:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC4));
			break;
		case 48:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC4));
			break;
		}
	}
}

// Initialise spi module
void spi_init(LPC_SPI_T* module, _U32 bitrate,
				SPI_CLOCK_MODE_T clock_mode,
				SPI_DATA_ORDER data_order) {

	Chip_SPI_Init(module);

	SPI_CFGSETUP_T cfg;
	cfg.master = 1;
	cfg.lsbFirst = data_order;
	cfg.mode = clock_mode;

	SPIM_DELAY_CONFIG_T dcfg;
	dcfg.PreDelay = 0xD;
	dcfg.PostDelay = 0xD;
	dcfg.FrameDelay = 0xD;
	dcfg.TransferDelay = 0xD;

	Chip_SPI_ConfigureSPI(module, &cfg);

	Chip_SPIM_SetClockRate(module, bitrate);

	Chip_SPIM_DelayConfig(module, &dcfg);

	Chip_SPI_Enable(module);
}

// Deinitialise SPI module
void spi_deinit(LPC_SPI_T* module) {
	Chip_SPI_Disable(module);
	Chip_SPI_DeInit(module);
}

// Send num_bytes in data array via spi
void spi_sendBytes(LPC_SPI_T* module, _U08 *data, _U08 num_bytes) {
	SPIM_XFER_T cfg;
	cfg.cbFunc = NULL;
	cfg.txBuff = data;
	cfg.rxBuff = NULL;
	cfg.txCount = num_bytes;
	cfg.rxCount = 0;
	cfg.txDoneCount = 0;
	cfg.rxDoneCount = 0;
	cfg.state = SPIS_XFER_STATE_IDLE;
	cfg.options = SPIM_XFER_OPTION_EOF | SPIM_XFER_OPTION_SIZE(8);
	Chip_SPIM_XferBlocking(module, &cfg);
}

// Receives num_bytes into data array via SPI
void spi_receiveBytes(LPC_SPI_T* module, _U08 *data, _U08 num_bytes) {
	SPIM_XFER_T cfg;
	cfg.cbFunc = NULL;
	cfg.txBuff = NULL;
	cfg.rxBuff = data;
	cfg.txCount = 0;
	cfg.rxCount = num_bytes;
	cfg.txDoneCount = 0;
	cfg.rxDoneCount = 0;
	cfg.state = SPIS_XFER_STATE_IDLE;
	cfg.options = SPIM_XFER_OPTION_EOF | SPIM_XFER_OPTION_SIZE(8);
	Chip_SPIM_XferBlocking(module, &cfg);
}

// Sends and receives data simultaneously via spi
void spi_sendReceiveBytes(LPC_SPI_T* module, _U08 *tx_data, _U08 *rx_data, _U08 num_tx, _U08 num_rx) {
	SPIM_XFER_T cfg;
	cfg.cbFunc = NULL;
	cfg.txBuff = tx_data;
	cfg.rxBuff = rx_data;
	cfg.txCount = num_tx;
	cfg.rxCount = num_rx;
	cfg.txDoneCount = 0;
	cfg.rxDoneCount = 0;
	cfg.state = SPIS_XFER_STATE_IDLE;
	cfg.options = SPIM_XFER_OPTION_EOF | SPIM_XFER_OPTION_SIZE(8);
	Chip_SPIM_XferBlocking(module, &cfg);
}
