/*
 * project_config.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef PROJECT_CONFIG_H_
#define PROJECT_CONFIG_H_

// Clock settings
#define EXT_CLOCK
#define CLOCK_RATE 100000000

// PMOLED settings
#define PM_HEIGHT 64
#define PM_WIDTH 128

#define PM_SPI LPC_SPI1
#define MAX_CONSECUTIVE_BYTES 16
#define PM_SPI_BITRATE 8000000

// Button constants
#define RESTART_PININT 0
#define PAUSE_PININT 1
#define PLAY_PININT 2
#define UP_PININT 3
#define DOWN_PININT 4

// Motor constants
#define PWM_FREQUENCY 1000
#define POT_ADC_CHANNEL 1
#define MOTOR_SAMPLE_PERIOD 1000
#define MOTOR_THRESHOLD 16
#define MOTOR_GAIN 1

#define MOTOR_PAGE1_POS 250
#define MOTOR_PAGE_POS_DIFF 600

// MIC Settings
#define MIC_ADC_CHANNEL 8
#define MIC_THRESHOLD 1e10

// USB Constants
#define USB_UART LPC_USART0
#define UART_BAUDRATE 115200

// Flash storage constants
#define SONG_TABLE_PAGE 0xF0
#define SONG_DATA_PAGE 0x100
#define SONG_TABLE_BASE 0xF000
#define SONG_DATA_BASE 0x10000
#define SONG_TABLE_SIZE 4096
#define MAX_SONGS 128
#define MAX_CHORDS 24576
#define CHORDS_IN_PAGE 32
#define TOTAL_PAGES 0x300

#define COPY_SIZE 256 // For simplicity only copy 256 bytes at a time
#define BUFFER_SIZE 4096
#define PAGE_ALIGNMENT 256

// Song constants
#define MAX_NOTES_IN_CHORD 4
#define MAX_CHORDS_IN_SONG 4096
#define MAX_TURNS 4
#define MAX_PAGES 5 // Number of pages (2 sheets side by side)
// Equal to MAX_TURNS + 1

#endif /* PROJECT_CONFIG_H_ */
