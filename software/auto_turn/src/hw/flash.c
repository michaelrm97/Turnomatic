/*
 * flash.c
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#include <flash.h>
#include <string.h>

volatile _U08 flash_buffer[BUFFER_SIZE] __attribute__((aligned(PAGE_ALIGNMENT)));

void flash_copy(_U32 addr, _U32 size) {

	_U08 sector = addr >> 15; // divide by 32k
	Chip_IAP_PreSectorForReadWrite(sector, sector);
	Chip_IAP_CopyRamToFlash(addr, (_U32 *) flash_buffer, size);
}

void flash_read(_U32 addr, _U32 size) {
	memcpy((void *)addr, (void *)flash_buffer, size);
}
