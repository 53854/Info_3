#define F_CPU 16E6			// or F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>


#define LED_ON PORTC |= (1<<1)
#define LED_OFF PORTC &= ~(1<<1)
#define LED1GN_TOGGLE PINC |= (1<<1)
#define LED2GN_TOGGLE PINC |= (1<<2)
#define LED3GN_TOGGLE PINC |= (1<<3)
#define LED4GN_TOGGLE PINC |= (1<<4)

#define BUTTON_ONE_PRESS (!(PIND & (1<<1)))
#define BUTTON_TWO_PRESS (!(PINB & (1<<1)))

unsigned blinkTimes = 0;

void init(void){
	// LED
	DDRB |= (1<<1);		// Configure PB5 as Output
	
	// Buttons
	DDRB &= ~(1<<1);	//Configure PB1 as Input
	PORTB |= 1<<1;		//Enable Internal Pull-Up at PB1
	
	DDRD &= ~(1<<1);	//Configure PD1 as Input
	PORTD |= 1<<1;		//Enable Internal Pull-Up at PD1
}

void blink(){
	for(unsigned i = 0; i < blinkTimes; i++){
		LED_ON;
		_delay_ms(500);
		LED_OFF;
		_delay_ms(500);
	}
}

int main(void){
	init();
	
	while (1){
		if(BUTTON_ONE_PRESS){
			blink(blinkTimes ++);
		} else if (BUTTON_TWO_PRESS){
			blinkTimes = 0;	
		}
	}
}