/*
 * pmoleds.c
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#include <pindefs.h>

#include <pmoleds.h>
#include <pm_graphics.h>

#include <spi.h>
#include <gpio.h>
#include <timer.h>

// Relevant constants for commands
#define PM_WRITE_COMMAND 0
#define PM_WRITE_DATA 1

#define PM_RESET 0
#define PM_NORMAL 1

#define PM_CS_DISABLE 1
#define PM_CS_ENABLE 0

// Commands for SSD1306
#define SSD1306_NUM_INIT_COMMANDS 12
#define SSD1306_SET_CONTRAST 0x81

#define SSD1306_SET_ADDRESS_MODE 0x20
#define SSD1306_HOR_ADDRESS 0x00
#define SSD1306_VER_ADDRESS 0x01
#define SSD1306_PAGE_ADDRESS 0x10

#define SSD1306_SET_COL_ADDRESS 0x21
#define SSD1306_SET_PAGE_ADDRESS 0x22

static void pm_command(_U08 *command, _U08 num_bytes) {

	gpio_setPinValue(PM_DC_PORT, PM_DC_PIN, PM_WRITE_COMMAND); // Set to write command
	gpio_setPinValue(PM_CS_PORT, PM_CS_PIN, PM_CS_ENABLE);
	spi_sendBytes(PM_SPI, command, num_bytes);
	gpio_setPinValue(PM_CS_PORT, PM_CS_PIN, PM_CS_DISABLE);

}

// Public functions for actually writing to PMOLEDS via SPI
// Initialize PMOLEDs
void pm_init(void) {

	spi_sck_pinassign(PM_SPI, PM_CLK_PORT, PM_CLK_PIN);
	spi_mosi_pinassign(PM_SPI, PM_MOSI_PORT, PM_MOSI_PIN);
	spi_init(PM_SPI, PM_SPI_BITRATE, SPI_CLOCK_MODE0, SPI_DATA_MSB_FIRST);

	gpio_setPinDir(PM_CS_PORT, PM_CS_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(PM_CS_PORT, PM_CS_PIN, GPIO_MODE_NONE);
	gpio_setPinValue(PM_CS_PORT, PM_CS_PIN, PM_CS_DISABLE);

	gpio_setPinDir(PM_DC_PORT, PM_DC_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(PM_DC_PORT, PM_DC_PIN, GPIO_MODE_NONE);
	gpio_setPinValue(PM_DC_PORT, PM_DC_PIN, PM_WRITE_COMMAND);

	gpio_setPinDir(PM_RES_PORT, PM_RES_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinMode(PM_RES_PORT, PM_RES_PIN, GPIO_MODE_NONE);
	gpio_setPinValue(PM_RES_PORT, PM_RES_PIN, PM_RESET);

	timer_delay_us(10); // Minimum reset low pulse width

	gpio_setPinValue(PM_RES_PORT, PM_RES_PIN, PM_NORMAL);

	pm_clear();

	_U08 commands_ssd1306[SSD1306_NUM_INIT_COMMANDS][2] = {
		{0xA8, 0x3F}, // Set MUX ratio
		{0xD3, 0x00}, // Set display offset
		{0x40, 0x00}, // Set display start line
		{0xA1, 0x00}, // Set segment re-map
		{0xC8, 0x00}, // Set COM Output scan direction
		{0xDA, 0x12}, // Set COM Pins hardware configuration
		{0x81, 0x7F}, // Set contrast control
		{0xA4, 0x00}, // Disable entire display on
		{0xA6, 0x00}, // Set normal display
		{0xD5, 0x80}, // Set oscillator frequency
		{0x8D, 0x14}, // Enable charge pump regulator
		{0xAF, 0x00} // Display on
	};

	_U08 command_sizes_ssd1306[SSD1306_NUM_INIT_COMMANDS] = {2, 2, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1};

	for (int i = 0; i < SSD1306_NUM_INIT_COMMANDS; i++) {
		pm_command(commands_ssd1306[i], command_sizes_ssd1306[i]);
	}

}

void pm_set_contrast(_U08 level) {
	_U08 command[2];

	command[0] = SSD1306_SET_CONTRAST;
	command[1] = (_U08) level;

	return pm_command(command, 2);
}

void pm_clear(void) {
	_U08 command[3];
	_U08 data[MAX_CONSECUTIVE_BYTES] = {0};

	// Set horizontal addressing mode
	command[0] = SSD1306_SET_ADDRESS_MODE;
	command[1] = SSD1306_HOR_ADDRESS;

	pm_command(command, 2);

	// Set column addresses to whole address space
	command[0] = SSD1306_SET_COL_ADDRESS;
	command[1] = 0;
	command[2] = PM_WIDTH - 1;

	pm_command(command, 3);

	// Set page addresses to whole address space
	command[0] = SSD1306_SET_PAGE_ADDRESS;
	command[1] = 0;
	command[2] = PM_HEIGHT / 8 - 1;

	pm_command(command, 3);

	// Set line to write data
	gpio_setPinValue(PM_DC_PORT, PM_DC_PIN, PM_WRITE_DATA);
	gpio_setPinValue(PM_CS_PORT, PM_CS_PIN, PM_CS_ENABLE);
	// Number of 0s to write to display
	int size = PM_WIDTH * PM_HEIGHT / 8;
	for (int i = 0; i < size; i += MAX_CONSECUTIVE_BYTES) {
		spi_sendBytes(PM_SPI, data, MAX_CONSECUTIVE_BYTES);
	}
	gpio_setPinValue(PM_CS_PORT, PM_CS_PIN, PM_CS_DISABLE);
}

void pm_write_buffer(void) {
	_U08 command[3];

	// Set horizontal addressing mode1
	command[0] = SSD1306_SET_ADDRESS_MODE;
	command[1] = SSD1306_HOR_ADDRESS;

	pm_command(command, 2);

	// Set column addresses to whole address space
	command[0] = SSD1306_SET_COL_ADDRESS;
	command[1] = 0;
	command[2] = PM_WIDTH - 1;

	pm_command(command, 3);

	// Set page addresses to whole address space
	command[0] = SSD1306_SET_PAGE_ADDRESS;
	command[1] = 0;
	command[2] = PM_HEIGHT / 8 - 1;

	pm_command(command, 3);

	// Set line to write data
	gpio_setPinValue(PM_DC_PORT, PM_DC_PIN, PM_WRITE_DATA);
	gpio_setPinValue(PM_CS_PORT, PM_CS_PIN, PM_CS_ENABLE);
	for (int i = 0; i < PM_HEIGHT / 8; i++) {
		for (int j = 0; j < PM_WIDTH; j += MAX_CONSECUTIVE_BYTES) {
			int num_bytes = MAX_CONSECUTIVE_BYTES;
			if (PM_WIDTH - j < MAX_CONSECUTIVE_BYTES) {
				num_bytes = PM_WIDTH - j;
			}
			spi_sendBytes(PM_SPI, &pm_buffer[i][j], num_bytes);
		}
	}
	gpio_setPinValue(PM_CS_PORT, PM_CS_PIN, PM_CS_DISABLE);
}

