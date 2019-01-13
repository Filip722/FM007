/*
 * main.h
 *
 * Created: 10/7/2018 1:49:33 PM
 *  Author: Filip Michalski
 */ 


#ifndef MAIN_H_
#define MAIN_H_


#define F_CPU 8000000
#define PWM_MAX 16383 // 2^14 - 1

#define BALANCE_MAX 1024
#define BALANCE_MIN 0
#define CH1 OCR0A
#define CH2 OCR0B
#define BUTTON !(PINB & (1 << PB2))
#define MONO 1
#define DW 2
#define GLOW 3

#define CHANGE_BRIGHTNESS_DELAY 50
#define CHANGE_BALANCE_DELAY 2
#define TURNONOFF_DELAY 1
#define DELAY_TURN_VS_BRIGHTNESS 300
#define PWM_MIN PWM_MAX/10
#define DEBOUNCE_TIME 30
#define DYNAMIC_DIMMING 1
#define DYNAMIC_BALANCE 1
#define DELAY_ONOFF 500


#endif /* MAIN_H_ */