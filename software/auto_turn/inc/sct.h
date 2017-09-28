/*
 * sct.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef SCT_H_
#define SCT_H_

#include <chip.h>
#include <compiler.h>

// Initialise sct module
void sct_init(void);

// Set/ unset sct to tick at a given frequency
void sct_set_periodic(_U32 frequency);
void sct_unset_periodic(void);

#endif /* SCT_H_ */
