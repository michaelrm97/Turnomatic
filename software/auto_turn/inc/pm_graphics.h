/*
 * pm_graphics.h
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#ifndef PM_GRAPHICS_H_
#define PM_GRAPHICS_H_

#include <compiler.h>
#include <project_config.h>

#define NUM_CHARS 128
#define CHAR_ROWS 1
#define CHAR_HEIGHT 8
#define CHAR_WIDTH 5

typedef struct {
	void *img;
	int width;
	int height;
} pm_image;

extern _U08 pm_buffer[PM_HEIGHT/8][PM_WIDTH];
extern _U08 (*font[NUM_CHARS])[CHAR_ROWS][CHAR_WIDTH];

void pm_draw_pixel(int x, int y);
void pm_clear_pixel(int x, int y);
void pm_draw_hline(int x1, int x2, int y);
void pm_draw_vline(int x, int y1, int y2);
void pm_draw_rectangle(int x1, int y1, int x2, int y2);
void pm_fill_rectangle(int x1, int y1, int x2, int y2);
void pm_clear_rectangle(int x1, int y1, int x2, int y2);

void pm_place_image(pm_image img, int x, int y);
void pm_place_character(char c, int x, int y);
void pm_place_string(char *s, int x, int y);

void pm_invert_section(int x1, int y1, int x2, int y2);

#endif /* PM_GRAPHICS_H_ */