/*
* FM007 Attiny10 2x LED DRIVER.c
*
* Created: 10/7/2018 1:25:16 PM
* Author : Filip Michalski
*/

#include "main.h"
#include <avr/io.h>
#include <util/delay.h>

uint8_t state = 0, direction_brightness = BOOT_DIRECTION_BRIGHTNESS, direction_balance = BOOT_DIRECTION_BALANCE;
uint16_t brightness = BOOT_BRIGHTNESS, balance = BOOT_BALANCE;

void Set_Brightness() {

	uint32_t ExpVal = ((uint32_t)brightness * (uint32_t)brightness) >> 14;
	if(ExpVal > PWM_MAX)
	ExpVal = PWM_MAX;

	switch(MODE) {
		case MONO:
		CH1 = ExpVal;
		CH2 = ExpVal;
		break;
		case DW:
		CH1 = (ExpVal * balance) >> 10;
		CH2 = ExpVal - CH1;
		break;
		case GLOW:
		CH1 = ExpVal;
		CH2 = (ExpVal * ExpVal) >> 14 ;
		break;
	}
}

void TurnOnOff() {
	uint16_t temp = brightness;
	
	if(state) { // turn off
		for(; brightness > 0; brightness--) {
			Set_Brightness();
			_delay_us(TURNONOFF_DELAY);
		}
		} else { //turn on
		for(brightness = 0;  brightness < temp; brightness++) {
			Set_Brightness();
			_delay_us(TURNONOFF_DELAY);
		}
	}
	brightness = temp;
	state ^= 1;
}

void Change_Brightness() {
	if(!state) {
		for(brightness = 0; brightness < PWM_MIN; brightness++) {
			Set_Brightness();
			_delay_us(CHANGE_BRIGHTNESS_DELAY);
		}
		state = 1;
		direction_brightness = 1;
	}

	if(direction_brightness && brightness < PWM_MAX) {
		brightness++;
		Set_Brightness();
		} else if(!direction_brightness && brightness > PWM_MIN) {
		brightness--;
		Set_Brightness();
	}

	if(brightness == PWM_MAX || brightness == PWM_MIN) {
		while(BUTTON) {};
	}
	_delay_us(CHANGE_BRIGHTNESS_DELAY);
}

void Change_Balance() {
	if(state) {
		if(direction_balance && balance < BALANCE_MAX) {
			balance++;
			Set_Brightness();
			} else if(!direction_balance && balance > BALANCE_MIN) {
			balance--;
			Set_Brightness();
		}
	}
	if(balance == BALANCE_MAX || balance == BALANCE_MIN) {
		while(BUTTON) {};
	}
	_delay_ms(CHANGE_BALANCE_DELAY);
}

int main(void) {
	uint8_t delay_counter = 0;

	CCP = 0xD8; // turn off security to change prescaler
	CLKPSR = 0; // prescaler = 0, clock == 8mhz
	DDRB = (1<<PORTB1 | 1<<PORTB0); // PB0 & PB1 as output
	PUEB = 1<<PORTB2; // PB2 as input with pull-up
	TCCR0A = (1<<COM0A1 | 1<<COM0B1 | 1<<WGM01); // PWM output 1 & 2, Timer0 Mode == fastPWM
	TCCR0B = (1<<WGM03 | 1<< WGM02 | 1<<CS00 ); // Timer Prescaler = 0
	ICR0 = PWM_MAX+1; // 2^14 bits,

	if(BOOT_STATE)
	TurnOnOff();



	while (1) {
		if(BUTTON) {
			_delay_ms(DEBOUNCE_TIME);
			if(BUTTON) {
				if(!state) {
					_delay_ms(DELAY_TURN_VS_BRIGHTNESS);
					if(BUTTON) {
						while(BUTTON)
						Change_Brightness();
						direction_brightness ^= 1;
					} else
					TurnOnOff();
					} else {
					delay_counter = 0;
					while(BUTTON && delay_counter < 10) {
						delay_counter++;
						_delay_ms(25);
					}
					if(delay_counter == 10) {
						while(BUTTON)
						Change_Brightness();
						direction_brightness ^= 1;
						} else {
						delay_counter = 0;
						while(!BUTTON && delay_counter < 10) {
							delay_counter++;
							_delay_ms(25);
						}
						if(BUTTON && MODE == DW){
							while(BUTTON)
							Change_Balance();
							direction_balance ^= 1;
						}
						
						else
						TurnOnOff();
					}
				}
			}
		}
	}
}