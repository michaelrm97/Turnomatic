/*
 * flash.h
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <compiler.h>

#include <chip.h>
#include <iap.h>

#define BUFFER_SIZE 4096

extern _U08 flash_buffer[BUFFER_SIZE];

void flash_copy(_U32 addr, _U32 size);

#endif /* FLASH_H_ */
