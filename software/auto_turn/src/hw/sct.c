/*
 * sct.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <sct.h>
#include <gpio.h>

#include <sct_5410x.h>

void sct_init(void) {
	Chip_SCT_Init(LPC_SCT0);
	Chip_SCT_Config(LPC_SCT0, SCT_CONFIG_32BIT_COUNTER | SCT_CONFIG_AUTOLIMIT_L);
}

// Period in 0.5us
void sct_set_periodic(_U32 frequency) {
	Chip_SCT_SetMatchCount(LPC_SCT, SCT_MATCH_0, SystemCoreClock / frequency);
	Chip_SCT_SetMatchReload(LPC_SCT, SCT_MATCH_0, SystemCoreClock / frequency);
	LPC_SCT->EVENT[0].CTRL = (1 << 12);
	LPC_SCT->EVENT[0].STATE = 0x00000001;
	Chip_SCT_SetMatchCount(LPC_SCT, SCT_MATCH_1, (SystemCoreClock / frequency) - 1);
	Chip_SCT_SetMatchReload(LPC_SCT, SCT_MATCH_1, (SystemCoreClock / frequency) - 1);
	LPC_SCT->EVENT[1].CTRL = (1 << 12) | (1 << 1);
	LPC_SCT->EVENT[1].STATE = 0x00000001;

	LPC_SCT->OUT[7].SET = (1 << 1);
	LPC_SCT->OUT[7].CLR = (1 << 0);

	LPC_SCT->LIMIT_U = 0x00000001;

	Chip_SCT_ClearControl(LPC_SCT, SCT_CTRL_HALT_L);
}

void sct_unset_periodic(void) {
	Chip_SCT_SetControl(LPC_SCT, SCT_CTRL_HALT_L);
}

