/*
 * user.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <pindefs.h>
#include <project_config.h>

#include <user.h>

#include <mode.h>
#include <gpio.h>
#include <pmoleds.h>
#include <pm_graphics.h>

void user_init(void) {

	// Setup LEDs
	gpio_setPinDir(LED_STOPPED_PORT, LED_STOPPED_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinDir(LED_PAUSED_PORT, LED_PAUSED_PIN, GPIO_DIR_OUTPUT);
	gpio_setPinDir(LED_RUNNING_PORT, LED_RUNNING_PIN, GPIO_DIR_OUTPUT);

	gpio_setPinMode(LED_STOPPED_PORT, LED_STOPPED_PIN, GPIO_MODE_NONE);
	gpio_setPinMode(LED_PAUSED_PORT, LED_PAUSED_PIN, GPIO_MODE_NONE);
	gpio_setPinMode(LED_RUNNING_PORT, LED_RUNNING_PIN, GPIO_MODE_NONE);

	gpio_setPinValue(LED_STOPPED_PORT, LED_STOPPED_PIN, 0);
	gpio_setPinValue(LED_PAUSED_PORT, LED_PAUSED_PIN, 0);
	gpio_setPinValue(LED_RUNNING_PORT, LED_RUNNING_PIN, 0);

	// Setup Buttons
	gpio_assignInterrupt(RESTART_BUTTON_PORT, RESTART_BUTTON_PIN,
			RESTART_PININT, GPIO_MODE_PULLUP, GPIO_INT_MODE_EDGE,
			GPIO_INT_EDGE_FALLING, GPIO_INT_LEVEL_LOW);

	gpio_assignInterrupt(PAUSE_BUTTON_PORT, PAUSE_BUTTON_PIN,
			PAUSE_PININT, GPIO_MODE_PULLUP, GPIO_INT_MODE_EDGE,
			GPIO_INT_EDGE_FALLING, GPIO_INT_LEVEL_LOW);

	gpio_assignInterrupt(PLAY_BUTTON_PORT, PLAY_BUTTON_PIN,
			PLAY_PININT, GPIO_MODE_PULLUP, GPIO_INT_MODE_EDGE,
			GPIO_INT_EDGE_FALLING, GPIO_INT_LEVEL_LOW);

	gpio_assignInterrupt(UP_BUTTON_PORT, UP_BUTTON_PIN,
			UP_PININT, GPIO_MODE_PULLUP, GPIO_INT_MODE_EDGE,
			GPIO_INT_EDGE_FALLING, GPIO_INT_LEVEL_LOW);

	gpio_assignInterrupt(DOWN_BUTTON_PORT, DOWN_BUTTON_PIN,
			DOWN_PININT, GPIO_MODE_PULLUP, GPIO_INT_MODE_EDGE,
			GPIO_INT_EDGE_FALLING, GPIO_INT_LEVEL_LOW);

	// Setup PMOLED
    pm_init();
    pm_place_string("Turnomatic", 10, 10);
    pm_write_buffer();

}

void user_mode_set(MODE smode) {
	switch(smode) {
	case MODE_PLAYING:
		gpio_enableInterrupt(RESTART_PININT, TRUE);
		gpio_enableInterrupt(PAUSE_PININT, TRUE);
		gpio_enableInterrupt(PLAY_PININT, FALSE);
		gpio_enableInterrupt(UP_PININT, FALSE);
		gpio_enableInterrupt(DOWN_PININT, FALSE);
		gpio_setPinValue(LED_STOPPED_PORT, LED_STOPPED_PIN, 0);
		gpio_setPinValue(LED_PAUSED_PORT, LED_PAUSED_PIN, 0);
		gpio_setPinValue(LED_RUNNING_PORT, LED_RUNNING_PIN, 1);
		break;
	case MODE_PAUSED:
		gpio_enableInterrupt(RESTART_PININT, TRUE);
		gpio_enableInterrupt(PAUSE_PININT, FALSE);
		gpio_enableInterrupt(PLAY_PININT, TRUE);
		gpio_enableInterrupt(UP_PININT, TRUE);
		gpio_enableInterrupt(DOWN_PININT, TRUE);
		gpio_setPinValue(LED_STOPPED_PORT, LED_STOPPED_PIN, 0);
		gpio_setPinValue(LED_PAUSED_PORT, LED_PAUSED_PIN, 1);
		gpio_setPinValue(LED_RUNNING_PORT, LED_RUNNING_PIN, 0);
		break;
	case MODE_STOPPED:
		gpio_enableInterrupt(RESTART_PININT, TRUE);
		gpio_enableInterrupt(PAUSE_PININT, TRUE);
		gpio_enableInterrupt(PLAY_PININT, TRUE);
		gpio_enableInterrupt(UP_PININT, TRUE);
		gpio_enableInterrupt(DOWN_PININT, TRUE);
		gpio_setPinValue(LED_STOPPED_PORT, LED_STOPPED_PIN, 1);
		gpio_setPinValue(LED_PAUSED_PORT, LED_PAUSED_PIN, 0);
		gpio_setPinValue(LED_RUNNING_PORT, LED_RUNNING_PIN, 0);
		break;
	case MODE_LOADING:
		gpio_enableInterrupt(RESTART_PININT, FALSE);
		gpio_enableInterrupt(PAUSE_PININT, FALSE);
		gpio_enableInterrupt(PLAY_PININT, FALSE);
		gpio_enableInterrupt(UP_PININT, FALSE);
		gpio_enableInterrupt(DOWN_PININT, FALSE);
		gpio_setPinValue(LED_STOPPED_PORT, LED_STOPPED_PIN, 1);
		gpio_setPinValue(LED_PAUSED_PORT, LED_PAUSED_PIN, 0);
		gpio_setPinValue(LED_RUNNING_PORT, LED_RUNNING_PIN, 0);
		break;
	}
}

// RESTART
void PIN_INT0_IRQHandler(void) {
	switch(mode) {
	case MODE_PLAYING:
		// Stop tracking of song
	case MODE_PAUSED:
		// Go back to bar 1
		// If second press, go to stopped mode
	case MODE_STOPPED:
		// Display song list with first song selected
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
	user_mode_set(MODE_STOPPED);
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT0));
}

// PAUSE
void PIN_INT1_IRQHandler(void) {
	switch(mode) {
	case MODE_PLAYING:
		// Stop tracking of song
	case MODE_PAUSED:
		// Do nothing
		break;
	case MODE_STOPPED:
		// Delete currently selected song (confirm)
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
	user_mode_set(MODE_PAUSED);
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT1));
}

// PLAY
void PIN_INT2_IRQHandler(void) {
	switch(mode) {
	case MODE_PLAYING:
		// Do nothing
		break;
	case MODE_PAUSED:
		// Resume song tracking
		break;
	case MODE_STOPPED:
		// Play currently selected song
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
	user_mode_set(MODE_PLAYING);
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT2));
}

// UP
void PIN_INT3_IRQHandler(void) {
	switch(mode) {
	case MODE_PLAYING:
	case MODE_PAUSED:
		// Increase bar number
		break;
	case MODE_STOPPED:
		// Move song select up
		// Confirm song deletion
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT3));
}

// DOWN
void PIN_INT4_IRQHandler(void) {
	switch(mode) {
	case MODE_PLAYING:
	case MODE_PAUSED:
		// Decrease bar number
		break;
	case MODE_STOPPED:
		// Decrease bar number
		// Reject song deletion
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT4));
}
