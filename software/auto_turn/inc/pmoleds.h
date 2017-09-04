/*
 * pmoleds.h
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#ifndef PMOLEDS_H_
#define PMOLEDS_H_

#include <compiler.h>
#include <project_config.h>

// Public functions for actually writing to PMOLEDS via SPI
// Initialize PMOLEDs
void pm_init(void);

void pm_set_contrast(_U08 level);

void pm_clear(void);

void pm_write_section(_U08 x1, _U08 y1, _U08 x2, _U08 y2);
void pm_write_buffer(void);

#endif /* PMOLEDS_H_ */
