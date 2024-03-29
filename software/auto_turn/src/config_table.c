/*
 * config_table.c
 *
 *  Created on: 4 Oct 2017
 *      Author: Michael
 */

#include <cr_section_macros.h>

#include <project_config.h>

#include <config_table.h>
#include <flash.h>

#include <string.h>

__RODATA(Flash2) const _U32 config_table[4] = {
		DEFAULT_PAGE1_POS,
		DEFAULT_PAGE_POS_DIFF,
		DEFAULT_MOTOR_GAIN,
		DEFAULT_MIC_THRESHOLD
};

void config_set_values(_U32 * newValues) {
	flash_read(CONFIG_TABLE_BASE, 256);
	memcpy(flash_buffer, newValues, 16);
	flash_copy(CONFIG_TABLE_BASE, 0, 256);
}
