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
#include <ctype.h>

#define MAX_STR_LEN 32
#define MAX_SONGS_DISPLAY 8

int curr_selection;
int curr_start;
Note_t last_note = NO_NOTE;

static void num2str(int n, char *s) {
	if (n == 0) {
		s[0] = '0';
		s[1] = '\0';
	}
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

static void update_bar_number(void) {
	char str[4];
	pm_clear_rectangle(7, 12, 53, 35);
	num2str(curr_bar, str);
	int x = 55 - 16 * strlen(str);
	int i = 0;
	while (str[i]) {
		pm_place_image(digits[str[i] - '0'], x, 11);
		i++;
		x += 16;
	}
}

static void update_page_number(void) {
	char str[4];
	pm_clear_rectangle(26, 39, 40, 62);
	num2str(curr_page, str);
	int x = 42 - 16 * strlen(str);
	int i =  0;
	while (str[i]) {
		pm_place_image(digits[str[i] - '0'], x, 39);
		i++;
		x += 16;
	}
}

static void update_note(Note_t n) {
	pm_clear_rectangle(78, 39, 124, 62);
	if (n != NO_NOTE) {
		int x = 78;
		int i = 0;
		pm_place_image(note_images[(note_names[n][i] - 'A')], x, 39);
		x += 16;
		i++;
		if (note_names[n][i] == 'S') {
			pm_place_image(sharp, x, 39);
			x += 16;
			i++;
		} else if (note_names[n][i] == 'b') {
			pm_place_image(flat, x, 39);
			x += 16;
			i++;
		}

		pm_place_image(digits[(note_names[n][i] - '0')], x, 39);
	}
}

static void setup_play_display(char *name) {
	pm_clear_rectangle(0, 0, 127, 63);
	pm_place_string(name, 1, 1);
	pm_draw_hline(0, 127, 10);
	pm_draw_hline(0, 127, 37);
	pm_draw_vline(74, 38, 63);
	pm_place_image(slash1, 55, 11);
	pm_place_image(slash1, 42, 39);
	pm_place_character('B', 1, 12);
	pm_place_character('A', 1, 20);
	pm_place_character('R', 1, 28);
	pm_place_string("PAGE", 1, 47);
	char str[4];
	num2str(max_bar, str);
	int x = 71;
	int i = 0;
	while (str[i]) {
		pm_place_image(digits[str[i] - '0'], x, 11);
		i++;
		x += 16;
	}
	num2str(max_page, str);
	x = 58;
	i = 0;
	while (str[i]) {
		pm_place_image(digits[str[i] - '0'], x, 39);
		i++;
		x += 16;
	}
	update_bar_number();
	update_page_number();
	Note_t n = track_update();
	last_note = n;
	if (n != NO_NOTE) {
		update_note(n);
	}
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

void user_update(void) {

	Note_t n = track_update();
	if (n != last_note && n != NO_NOTE) {
		update_note(n);
		last_note = n;
		pm_write_buffer();
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
			update_bar_number();
			update_page_number();
			pm_write_buffer();
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
			track_set_song(song_load(curr_selection));
			setup_play_display(song_name_get(curr_selection));
			user_mode_set(MODE_PLAYING);
			track_begin();
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
		update_bar_number();
		update_page_number();
		pm_write_buffer();
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
		update_bar_number();
		update_page_number();
		pm_write_buffer();
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
