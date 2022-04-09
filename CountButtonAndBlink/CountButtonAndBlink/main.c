#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16E6			// or F_CPU 16000000

#define LED_ON PORTB |= (1<<5)
#define LED_OFF PORTB &= ~(1<<5)
#define LED_TOGGLE PINB |= (1<<5)

#define BUTTON_PRESS (!(PINB & (1<<7)))

unsigned buttonPresses = 0;

void init(void){
	// LED
	DDRB |= (1<<5);		// Configure PB5 as Output
	
	// Button
	DDRB &= ~(1<<7);	//Configure PB1 as Input
	PORTB |= 1<<7;		//Enable Internal Pull-Up at PB1
}

void blink(){
	for(unsigned i = 0; i <= buttonPresses; i++){
		LED_ON;
		_delay_ms(5000);
		LED_OFF;
		_delay_ms(5000);
	}
}

int main(void){
	init();
	
	while (1){
		if(BUTTON_PRESS){
			blink(buttonPresses ++);
		}
	}
}