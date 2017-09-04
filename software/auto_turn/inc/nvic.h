/*
 * nvic.h
 *
 *  Created on: 4 Sep 2017
 *      Author: Michael
 */

#ifndef NVIC_H_
#define NVIC_H_

#include <compiler.h>
#include <chip.h>
#include <cmsis.h>

void nvic_init(void);
void nvic_setInterruptVector(int irq_num, void *handler);
void nvic_setInterruptPriority(int irq_num, _U08 priority);

static inline void nvic_enableInterrupt(int irq_num){
	NVIC_EnableIRQ(irq_num);
}

static inline void nvic_disableInterrupt(int irq_num){
	NVIC_DisableIRQ(irq_num);
}

static inline void nvic_setInterrupt(int irq_num){
	NVIC_SetPendingIRQ(irq_num);
}

static inline void nvic_clearInterrupt(int irq_num){
	NVIC_ClearPendingIRQ(irq_num);
}


#endif /* NVIC_H_ */
