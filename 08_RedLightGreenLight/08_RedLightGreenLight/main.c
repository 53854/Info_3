#include <avr/io.h>

#define GREEN_ON {PORTC |= 0b00111110; PORTD |= 0b00110000;}
#define GREEN_OFF {PORTC &= ~0b00111110; PORTD &= ~0b00110000;}
	
#define RED_ON PORTD	|= 0b11000001
#define RED_OFF PORTD	&= ~0b11000001

#define BUTTON_ONE_PRESS (!(PIND & (1<<1)))
#define BUTTON_TWO_PRESS (!(PINB & (1<<1)))

void init(void){
	// LEDS
	DDRC |= 0b00111110;
	DDRD |= 0b11110001;

	// Buttons
	DDRB &= ~(1<<1);	//Configure PB1 as Input
	PORTB |= 1<<1;		//Enable Internal Pull-Up at PB1
	
	DDRD &= ~(1<<1);	//Configure PD1 as Input
	PORTD |= 1<<1;		//Enable Internal Pull-Up at PD1
}

void check_buttons(void){
	if(BUTTON_ONE_PRESS)	{GREEN_ON;}
	else					{GREEN_OFF;}
	
	if(BUTTON_TWO_PRESS)	{RED_ON;}
	else					{RED_OFF;}
		
		
	
}

int main(void){
	init();
	while (1){
		check_buttons();
	}
}