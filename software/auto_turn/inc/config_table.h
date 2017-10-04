/*
 * config_table.h
 *
 *  Created on: 4 Oct 2017
 *      Author: Michael
 */

#ifndef CONFIG_TABLE_H_
#define CONFIG_TABLE_H_

#include <chip.h>
#include <compiler.h>

#define MOTOR_PAGE1_POS (*(int *)(&config_table[0]))
#define MOTOR_PAGE_POS_DIFF (*(int *)(&config_table[1]))
#define MOTOR_GAIN (*(float *)(&config_table[2]))
#define MIC_THRESHOLD (*(float *)(&config_table[3]))

void config_set_values(_U32 * newValues);

extern const _U32 config_table[4];

#endif /* CONFIG_TABLE_H_ */
