/*
 * sct.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <sct.h>

#include <sct_5410x.h>
#include <sct_pwm_5410x.h>

_U32 pwm_rate;

void sct_init(void) {
	Chip_SCT_Init(LPC_SCT0);
	Chip_SCT_Config(LPC_SCT0, SCT_CONFIG_32BIT_COUNTER | SCT_CONFIG_AUTOLIMIT_L);

}

// Period in 0.5us
void sct_set_periodic(_U32 period) {
	LPC_SCT0->MATCH[0].L = period - 1;
	LPC_SCT0->CTRL_L |= SCT_CTRL_CLRCTR_L;
	LPC_SCT0->CTRL_L &= ~(SCT_CTRL_HALT_L | SCT_CTRL_STOP_L);
}

void sct_unset_periodic(void) {
	LPC_SCT0->CTRL_L |= SCT_CTRL_STOP_L;
}
