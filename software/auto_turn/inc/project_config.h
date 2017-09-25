/*
 * project_config.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef PROJECT_CONFIG_H_
#define PROJECT_CONFIG_H_

#define EXT_CLOCK
// Clock settings
#define CLOCK_RATE 100000000

// PMOLED dimensions
#define PM_HEIGHT 64
#define PM_WIDTH 128

#define PM_SPI LPC_SPI1
#define MAX_CONSECUTIVE_BYTES 16
#define PM_SPI_BITRATE 8000000

// Motor constants
#define PWM_FREQUENCY 1000
#define POT_ADC_CHANNEL 1
#define MOTOR_SAMPLE_PERIOD 1000
#define MOTOR_THRESHOLD 16
#define MOTOR_GAIN 1

// Button constants
#define RESTART_PININT 0
#define PAUSE_PININT 1
#define PLAY_PININT 2
#define UP_PININT 3
#define DOWN_PININT 4

// USB Constants
#define USB_UART LPC_USART0
#define UART_BAUDRATE 115200

// MIC Settings
#define MIC_ADC_CHANNEL 8

#endif /* PROJECT_CONFIG_H_ */
