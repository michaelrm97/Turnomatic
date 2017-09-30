/*
 * flash.h
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <chip.h>
#include <compiler.h>

#include <project_config.h>

extern volatile _U08 flash_buffer[BUFFER_SIZE] __attribute__((aligned(PAGE_ALIGNMENT)));

// Copy from flash_buffer in ram to flash at addr (must be page aligned)
// Takes in a size to copy which must be either 256, 512, 1024 or 4096 bytes
// Returns address immediately after written flash
_U32 flash_copy(_U32 addr, _U32 offset, _U32 size);

// Copy from flash at addr to flash_buffer in ram (must be page aligned)
// Takes in a size to read which must be either 256, 512, 1024 or 4096 bytes
void flash_read(_U32 addr, _U32 size);

#endif /* FLASH_H_ */
