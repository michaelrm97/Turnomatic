/*
 * mode.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef MODE_H_
#define MODE_H_

typedef enum {
	MODE_PLAYING,
	MODE_PAUSED,
	MODE_STOPPED,
	MODE_LOADING
} MODE;

extern MODE mode;

#endif /* MODE_H_ */
