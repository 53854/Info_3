#define F_CPU 16E6			// or F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_ONE_PRESS (!(PIND & (1<<1)))
#define BUTTON_TWO_PRESS (!(PINB & (1<<1)))

unsigned counter;

void init(void){
	// counter
	counter = 0;
	
	// LED
	DDRC |= 0b00011110;		// Configure PC 1 - 4 as Output
	
	// Buttons
	DDRB &= ~(1<<1);	//Configure PB1 as Input
	PORTB |= 1<<1;		//Enable Internal Pull-Up at PB1
	
	DDRD &= ~(1<<1);	//Configure PD1 as Input
	PORTD |= 1<<1;		//Enable Internal Pull-Up at PD1
}


int main(void){
	init();
	
	while (1){
 		if(BUTTON_ONE_PRESS){
			counter ++;
			counter %= 16;
			PORTC = (counter << 1);
			while(BUTTON_ONE_PRESS){}
		} 
		if (BUTTON_TWO_PRESS){
			counter = 0;
			PORTC = (counter << 1);
		}
	}
}