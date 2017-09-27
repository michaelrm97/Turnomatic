/*
 * flash.c
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#include <flash.h>
#include <string.h>

volatile _U08 flash_buffer[BUFFER_SIZE] __attribute__((aligned(PAGE_ALIGNMENT)));

_U32 flash_copy(_U32 addr, _U32 size) {

	_U08 sector = addr >> 15; // divide by 32k
	if (Chip_IAP_PreSectorForReadWrite(sector, sector) != IAP_CMD_SUCCESS) {
		return addr;
	}
	if (size < 256) {
		size = 256;
	} else if (size > 256 && size < 512) {
		size = 512;
	} else if (size > 512 && size < 1024) {
		size = 1024;
	} else if (size > 1024 && size < 4096){
		size = 4096;
	} else {
		// Invalid size
		return addr;
	}
	if (Chip_IAP_CopyRamToFlash(addr, (_U32 *) flash_buffer, size) == IAP_CMD_SUCCESS) {
		return addr + size;
	} else {
		return addr;
	}
}

void flash_read(_U32 addr, _U32 size) {
	memcpy((void *)addr, (void *)flash_buffer, size);
}
