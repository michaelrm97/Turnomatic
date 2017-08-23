/*
 * pindefs.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef PINDEFS_H_
#define PINDEFS_H_

// USB-UART
#define ISP_PORT 0
#define ISP_PIN 31

#define UART_RX_PORT 0
#define UART_RX_PIN 0
#define UART_TX_PORT 0
#define UART_RX_PIN 1

// Microphone
#define MIC_ADC_PORT 1
#define MIC_ADC_PIN 5

// Motor + Pot
#define BUCK_EN_PIN 23
#define BUCK_EN_PORT 0

#define MOTOR_IN_1_PORT 0
#define MOTOR_IN_1_PIN 19
#define MOTOR_IN_2_PORT 0
#define MOTOR_IN_2_PIN 18

#define POT_ADC_PORT 0
#define POT_ACD_PIN 29

// PMOLED
#define SPI_CLK_PORT 1
#define SPI_CLK_PIN 12
#define SPI_MOSI_PORT 1
#define SPI_MOSI_PIN 13
#define SPI_RES_PORT 1
#define SPI_RES_PIN 14
#define SPI_DC_PORT 0
#define SPI_DC_PIN 20
#define SPI_CS1_PORT 1
#define SPI_CS1_PIN 16

// LEDs
#define LED_PAUSED_PORT 1
#define LED_PAUSED_PIN 4
#define LED_RUNNING_PORT 1
#define LED_RUNNING_PIN 3
#define LED_STOPPED_PORT 1
#define LED_STOPPED_PIN 2

// Buttons
#define RESTART_BUTTON_PORT 1
#define RESTART_BUTTON_PIN 1
#define PAUSE_BUTTON_PORT 1
#define PAUSE_BUTTON_PIN 0
#define PLAY_BUTTON_PORT 0
#define PLAY_BUTTON_PIN 30
#define UP_BUTTON_PORT 0
#define UP_BUTTON_PIN 25
#define DOWN_BUTTON_PORT 0
#define DOWN_BUTTON_PIN 24

#endif /* PINDEFS_H_ */
