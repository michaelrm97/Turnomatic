/*
 * user.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef USER_H_
#define USER_H_

#include <compiler.h>

#include <mode.h>

void user_init(void);

void user_mode_set(MODE smode);

void user_update(void);

void user_enter_loading(char *name, _U32 total);
void user_enter_deleting(char *name);
void user_update_loading(_U32 curr, _U32 total);
void user_update_deleting(_U08 percent);
void user_exit_loading(void);

#endif /* USER_H_ */
