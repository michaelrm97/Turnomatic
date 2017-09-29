/*
 * pm_graphics.c
 *
 *  Created on: Apr 16, 2017
 *      Author: Michael
 */

#include <chip.h>
#include <pm_graphics.h>

_U08 pm_buffer[PM_HEIGHT/8][PM_WIDTH];

// Draw single pixel
void pm_draw_pixel(int x, int y) {
	if (x >= 0 && y >= 0 && x < PM_WIDTH && y < PM_HEIGHT) {
		pm_buffer[y / 8][x] |= (1 << (y % 8));
	}
}

// Clear single pixel
void pm_clear_pixel(int x, int y) {
	if (x >= 0 && y >= 0 && x < PM_WIDTH && y < PM_HEIGHT) {
		pm_buffer[y / 8][x] &= ~(1 << (y % 8));
	}
}

// Draw horizontal line
void pm_draw_hline(int x1, int x2, int y) {
	if (x1 < 0) {
		x1 = 0;
	}
	if (x2 >= PM_WIDTH) {
		x2 = PM_WIDTH - 1;
	}
	if (y > PM_HEIGHT) {
		return;
	}
	while (x1 <= x2) {
		pm_draw_pixel(x1, y);
		x1++;
	}
}

// Draw vertical line
void pm_draw_vline(int x, int y1, int y2) {
	if (y1 < 0) {
		y1 = 0;
	}
	if (y2 >= PM_HEIGHT) {
		y2 = PM_HEIGHT - 1;
	}
	if (x > PM_WIDTH) {
		return;
	}
	while (y1 <= y2) {
		pm_draw_pixel(x, y1);
		y1++;
	}
}

// Draw a rectangle
void pm_draw_rectangle(int x1, int y1, int x2, int y2) {
	pm_draw_hline(x1, x2, y1);
	pm_draw_hline(x1, x2, y2);
	pm_draw_vline(x1, y1, y2);
	pm_draw_vline(x2, y1, y2);
}

// Fill a rectangle
void pm_fill_rectangle(int x1, int y1, int x2, int y2) {
	int page_start = y1 / 8;
	if (y1 < 0) {
		page_start--;
	}
	int page_end = y2 / 8;
	if (page_end > (PM_HEIGHT - 1) / 8) {
		page_end = (PM_HEIGHT - 1) / 8;
	}
	if (x1 < 0) {
		x1 = 0;
	}
	if (x2 >= PM_WIDTH) {
		x2 = PM_WIDTH - 1;
	}

	int i, j;
	for (i = page_start; i <= page_end; i ++) {
		for (j = x1; j <= x2; j++) {
			if (i == page_start) {
				pm_buffer[i][j] |= (0xFF << (y1 % 8));
			} else if (i == page_end) {
				pm_buffer[i][j] |= (0x00FF >> (7 - (y2 % 8)));
			} else {
				pm_buffer[i][j] = 0xFF;
			}
		}
	}
}

// Clear a rectangular area
void pm_clear_rectangle(int x1, int y1, int x2, int y2) {
	int page_start = y1 / 8;
	if (y1 < 0) {
		page_start--;
	}
	int page_end = y2 / 8;
	if (page_end > (PM_HEIGHT - 1) / 8) {
		page_end = (PM_HEIGHT - 1) / 8;
	}
	if (x1 < 0) {
		x1 = 0;
	}
	if (x2 >= PM_WIDTH) {
		x2 = PM_WIDTH - 1;
	}

	int i, j;
	for (i = page_start; i <= page_end; i ++) {
		for (j = x1; j <= x2; j++) {
			if (i == page_start) {
				pm_buffer[i][j] &= ~(0xFF << (y1 % 8));
			} else if (i == page_end) {
				pm_buffer[i][j] &= ~(0x00FF >> (7 - (y2 % 8)));
			} else {
				pm_buffer[i][j] = 0;
			}
		}
	}
}

// Place an image at specified location
void pm_place_image(pm_image img, int x, int y) {
	int page_start = y / 8;
	int shift = y % 8;
	if (y < 0) {
		page_start--;
		shift += 8;
	}

	int img_height = img.height;
	int img_width = img.width;
	int rows = (img_height + 7) / 8;

	_U08  (*img1)[img_width] = img.img;

	_U08 bits1 = ~(0xFF << shift);
	_U08 bits2 = ~(0xFF >> (8 - shift));
	for (int i = 0; i < rows; i++) {
		int page = page_start + i;
		if (i == rows - 1) {
			int remaining = img_height % 8;
			if (remaining > 0) {
				// Need to adjust bitmasks
				if (remaining < 8 - shift) {
					bits1 |= 0xFF << (remaining + shift);
					bits2 = 0xFF;
				} else {
					bits2 |= 0xFF << (remaining - shift);
				}

			}
		}
		for (int j = 0; j < img_width; j++) {
			int col = x + j;
			if (col >= 0 && col < PM_WIDTH) {
				// Place part-image on upper page
				if (page >= 0 && page < PM_HEIGHT / 8) {
					pm_buffer[page][col] &= bits1;
					pm_buffer[page][col] |= img1[i][j] << shift;
				}
				if (page + 1 >= 0 && page + 1 < PM_HEIGHT / 8) {
					pm_buffer[page + 1][col] &= bits2;
					pm_buffer[page + 1][col] |= img1[i][j] >> (8 - shift);
				}
			}
		}
	}
}

// Place character at specified location
void pm_place_character(char c, int x, int y) {
	pm_image img;
	if (c >= 0 && c < NUM_CHARS) {
		if (font[(int) c] != NULL) {
			img.img = font[(int) c];
			img.width = CHAR_WIDTH;
			img.height = CHAR_HEIGHT;
			pm_place_image(img, x, y);
		}
	}
}

// Place string at specified location
void pm_place_string(char *s, int x, int y) {
	int start_col = x;
	while (*s != '\0') {
		// Check that character can fit
		if (x + CHAR_WIDTH > PM_WIDTH) {
			// See if no characters can fit
			if (start_col == x) {
				return; // Give up printing
			} else {
				x = start_col;
				y += CHAR_HEIGHT;
			}
		}
		if (*s == '\n') {
			x = start_col;
			y += CHAR_HEIGHT;
		} else {
			pm_place_character(*s, x, y);
			x += CHAR_WIDTH + 1;
		}
		s++;
	}
}

// Invert section of pmoled buffer
void pm_invert_section(int x1, int y1, int x2, int y2) {
	int page_start = y1 / 8;
	if (y1 < 0) {
		page_start--;
	}
	int page_end = y2 / 8;
	if (page_end > (PM_HEIGHT - 1) / 8) {
		page_end = (PM_HEIGHT - 1) / 8;
	}
	if (x1 < 0) {
		x1 = 0;
	}
	if (x2 >= PM_WIDTH) {
		x2 = PM_WIDTH - 1;
	}

	int i, j;
	for (i = page_start; i <= page_end; i ++) {
		for (j = x1; j <= x2; j++) {
			if (i == page_start) {
				pm_buffer[i][j] ^= (0xFF << (y1 % 8));
			} else if (i == page_end) {
				pm_buffer[i][j] ^= (0x00FF >> (7 - (y2 % 8)));
			} else {
				pm_buffer[i][j] ^= 0xFF;
			}
		}
	}
}
