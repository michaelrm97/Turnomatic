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
	Chip_SCT_Config(LPC_SCT0, SCT_CONFIG_16BIT_COUNTER | SCT_CONFIG_AUTOLIMIT_L | SCT_CONFIG_AUTOLIMIT_H);
	_U08 prescale_l = Chip_Clock_GetSystemClockRate()/2000000 - 1; // 0.5us ticks
	_U08 prescale_h = Chip_Clock_GetSystemClockRate()/1000000 - 1; // 1us ticks
	Chip_SCT_SetControl(LPC_SCT0, SCT_CTRL_PRE_L(prescale_l) | SCT_CTRL_PRE_H(prescale_h));

	LPC_SCT0->REGMODE_L = 0;
	LPC_SCT0->REGMODE_H = 0;

	// Event 0 AND 1 for ADC
//	LPC_SCT0->EVENT[0].STATE = 0xFFFF;
//	LPC_SCT0->EVENT[0].CTRL = SCT_EV_CTRL_MATCHSEL(0) | SCT_EV_CTRL_HEVENT_L |
//			SCT_EV_CTRL_COMBMODE_MATCH | SCT_EV_CTRL_DIRECTION_INDEPENDENT;
//
//	LPC_SCT0->MATCH[1].L = 0;
//	LPC_SCT0->EVENT[1].STATE = 0xFFFF;
//	LPC_SCT0->EVENT[1].CTRL = SCT_EV_CTRL_MATCHSEL(1) | SCT_EV_CTRL_HEVENT_L |
//			SCT_EV_CTRL_COMBMODE_MATCH | SCT_EV_CTRL_DIRECTION_INDEPENDENT;
//
//	// Clear at timer = 0, set at timer = period - 1
//	LPC_SCT->OUT[7].SET = (1<<0);
//	LPC_SCT->OUT[7].CLR = (1<<1);
//
//	// Event 2 for ADC
//	LPC_SCT0->EVENT[2].STATE = 0xFFFF;
//	LPC_SCT0->EVENT[2].CTRL = SCT_EV_CTRL_MATCHSEL(0) | SCT_EV_CTRL_HEVENT_H |
//			SCT_EV_CTRL_COMBMODE_MATCH | SCT_EV_CTRL_DIRECTION_INDEPENDENT;

}

// Period in 0.5us
void sct_set_period(_U32 period) {
	LPC_SCT0->MATCH[0].L = period - 1;
	LPC_SCT0->CTRL_L |= SCT_CTRL_CLRCTR_L;
	LPC_SCT0->CTRL_L &= ~(SCT_CTRL_HALT_L | SCT_CTRL_STOP_L);
}

void sct_unset_period(void) {
	LPC_SCT0->CTRL_L |= SCT_CTRL_STOP_L;
}

void sct_pwm_init(_U32 frequency) {
	pwm_rate = 1000000 / frequency;
	Chip_SCT_SetControl(LPC_SCT0, SCT_CTRL_HALT_H);
	Chip_SCT_SetControl(LPC_SCT0, SCT_CTRL_CLRCTR_H);
	LPC_SCT0->MATCH[0].H = 0;
	LPC_SCT0->MATCHREL[0].H = pwm_rate;
	LPC_SCT0->EVENT[0].CTRL = SCT_EV_CTRL_HEVENT_H | (1 << 12);
	LPC_SCT0->EVENT[0].STATE = 1;
}

void sct_pwm_pinassign(_U08 port, _U08 pin, _U08 channel) {
	int i = port * 32 + pin;
	switch(channel) {
	case 0:
		switch(i) {
		case 7:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 18:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
		break;
	case 1:
		switch(i) {
		case 1:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC5));
			break;
		case 8:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 19:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
		break;
	case 2:
		switch(i) {
		case 9:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 29:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
		break;
	case 3:
		switch(i) {
		case 0:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC5));
			break;
		case 10:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 30:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
		break;
	case 4:
		switch(i) {
		case 13:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 33:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC3));
			break;
		}
		break;
	case 5:
		switch(i) {
		case 14:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 34:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC3));
			break;
		case 47:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		}
		break;
	case 6:
		switch(i) {
		case 5:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC2));
			break;
		case 35:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC3));
			break;
		}
		break;
	case 7:
		switch(i) {
		case 36:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC3));
			break;
		case 46:
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
				(IOCON_INPFILT_OFF | IOCON_DIGITAL_EN | IOCON_MODE_INACT | IOCON_FUNC3));
			break;
		}
		break;
	}
}

void sct_pwm_setOutPin(_U08 index, _U08 channel) {
	LPC_SCT0->EVENT[index].CTRL = SCT_EV_CTRL_HEVENT_H | index | (1 << 12);
	LPC_SCT0->EVENT[index].STATE = 1;
	LPC_SCT0->OUT[channel].SET = 1;
	LPC_SCT0->OUT[channel].CLR = 1 << index;
	LPC_SCT0->RES = (LPC_SCT0->RES & ~(3 << (channel << 1))) | (0x01 << (channel << 1));
	LPC_SCT0->OUTPUTDIRCTRL = (LPC_SCT0->OUTPUTDIRCTRL & ~(3 << (channel << 1)));
}

void sct_pwm_setPinDutyCycle(_U08 index, float duty) {
	_U32 ticks = pwm_rate * (100 - duty) / 100;
	LPC_SCT0->MATCHREL[index].H = ticks;
}

void sct_pwm_start(void) {
	Chip_SCT_ClearControl(LPC_SCT0, SCT_CTRL_HALT_H);
}

void sct_pwm_stop(void) {
	Chip_SCT_SetControl(LPC_SCT0, SCT_CTRL_HALT_H);
	Chip_SCT_SetControl(LPC_SCT0, SCT_CTRL_CLRCTR_H);
}
