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
#include <note_tracker.h>
#include <song_table.h>

#include <string.h>

#define MAX_STR_LEN 32
#define MAX_SONGS_DISPLAY 8

int curr_selection;
int curr_start;

static void num2str(int n, char *s) {
	int num = n;
	int digits = 0;
	while (num) {
		digits++;
		num /= 10;
	}
	s[digits--] = '\0';
	while (digits >= 0) {
		s[digits--] = (n % 10) + '0';
		n /= 10;
	}
}

static void display_bar_page_number(void) {
	char str[MAX_STR_LEN] = "Bar: ";
	num2str(curr_bar, str + 5);
	pm_clear_rectangle(10, 10, 127, 17);
	pm_place_string(str, 10, 10);
	strcpy(str, "Page: ");
	num2str(curr_page, str + 6);
	pm_clear_rectangle(10, 42, 127, 49);
	pm_place_string(str, 10, 42);
	pm_write_buffer();
}

static void display_page_number(void) {
	char str[MAX_STR_LEN] = "Page: ";
	num2str(curr_page, str + 6);
	pm_clear_rectangle(10, 42, 127, 49);
	pm_place_string(str, 10, 42);
	pm_write_buffer();
}

static void display_song_list(void) {

	pm_clear_rectangle(0, 0, 127, 63);

	char song_list[MAX_SONGS_DISPLAY][MAX_SONG_LEN];
	int num = song_list_get(song_list, MAX_SONGS_DISPLAY, curr_start);
	for (int i = 0; i < num ; i++) {
		pm_place_string(song_list[i], 2, 8 * i);
	}
	int y = curr_selection - curr_start;
	pm_invert_section(0, y * 8, 127, (y + 1) * 8 - 1);

	pm_write_buffer();

}

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

    user_mode_set(MODE_STOPPED);
    display_song_list();

}

void user_mode_set(MODE smode) {
	mode = smode;
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
		gpio_enableInterrupt(PAUSE_PININT, FALSE);
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
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT0));
	switch(mode) {
	case MODE_PLAYING:
		// Stop tracking of song
		track_stop();
	case MODE_PAUSED:
		if (curr_bar != 1) {
		// Go back to bar 1
			curr_bar = 1;
			curr_page = 1;
			display_bar_page_number();
			user_mode_set(MODE_PAUSED);
			break;
		} else {
		// If second press, go to stopped mode
			user_mode_set(MODE_STOPPED);
		}
	case MODE_STOPPED:
		// Display song list with first song selected
		curr_selection = 0;
		curr_start = 0;
		display_song_list();
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
}

// PAUSE
void PIN_INT1_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT1));
	switch(mode) {
	case MODE_PLAYING:
		// Stop tracking of song
		track_stop();
		user_mode_set(MODE_PAUSED);
	case MODE_PAUSED:
		// Do nothing
		break;
	case MODE_STOPPED:
		// Do nothing
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
}

static void begin_play(void) {
	track_set_song(song_load(curr_selection));
	pm_clear_rectangle(0, 0, 127, 63);
	pm_write_buffer();
	display_bar_page_number();
	user_mode_set(MODE_PLAYING);
	track_begin();
}

// PLAY
void PIN_INT2_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT2));
	switch(mode) {
	case MODE_PLAYING:
		// Do nothing
		break;
	case MODE_PAUSED:
		// Resume song tracking
		track_begin();
		user_mode_set(MODE_PLAYING);
		break;
	case MODE_STOPPED:
		// Play currently selected song
		if (curr_selection < song_num()) {
			begin_play();
		}
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
}

// UP
void PIN_INT3_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT3));
	switch(mode) {
	case MODE_PLAYING:
		// Do nothing
		break;
	case MODE_PAUSED:
		// Increase bar number
		curr_bar++;
		// Check if there's a need to change page
		display_bar_page_number();
		break;
	case MODE_STOPPED:
		if (curr_selection > 0) {
			curr_selection--;
			if (curr_start > curr_selection) {
				curr_start--;
			}
			display_song_list();
		}
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
}

// DOWN
void PIN_INT4_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(PININTSELECT4));
	switch(mode) {
	case MODE_PLAYING:
		// Do nothing
		break;
	case MODE_PAUSED:
		// Decrease bar number
		if (curr_bar > 1) {
			curr_bar--;
		}
		// Check if there's a need to change page
		display_bar_page_number();
		break;
	case MODE_STOPPED:
		// Move song select down
		if (curr_selection < song_num() - 1) {
			curr_selection++;
			if (curr_start + MAX_SONGS_DISPLAY - 1< curr_selection) {
				curr_start++;
			}
			display_song_list();
		}
		break;
	case MODE_LOADING:
		// Do nothing
		break;
	}
}
