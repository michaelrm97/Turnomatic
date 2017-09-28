/*
 * user.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef USER_H_
#define USER_H_

#include <chip.h>
#include <compiler.h>

#include <mode.h>

// Initialise user interface functions
// Including buttons and PMOLED display
void user_init(void);

// Set a given mode
// Activates/ deactivates relevant buttons and sets mode
void user_mode_set(MODE smode);

// Updates display whilst song is being played
// With new note, bar and page numbers
void user_update(void);

// Display song lodaing/ deletion info on screen
void user_enter_loading(char *name, _U32 total);
void user_enter_deleting(char *name);
void user_update_loading(_U32 curr, _U32 total);
void user_update_deleting(_U08 percent);
void user_exit_loading(void);

#endif /* USER_H_ */
