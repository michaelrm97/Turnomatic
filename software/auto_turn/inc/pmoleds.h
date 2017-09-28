/*
 * pmoleds.h
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#ifndef PMOLEDS_H_
#define PMOLEDS_H_

#include <chip.h>
#include <compiler.h>

// Public functions for actually writing to PMOLED via SPI
// Initialize screen
void pm_init(void);

// Set contrast level for screen
void pm_set_contrast(_U08 level);

// Clear screen
void pm_clear(void);

// Write buffer to screen
void pm_write_buffer(void);

#endif /* PMOLEDS_H_ */
