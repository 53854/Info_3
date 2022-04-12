#include <avr/io.h>
#include <avr/interrupt.h>

#define GREEN_ON {PORTC |= 0b00111110; PORTD |= 0b00110000;}
#define GREEN_OFF {PORTC &= ~0b00111110; PORTD &= ~0b00110000;}
#define GREEN_TOGGLE {PINC |= 0b00111110; PIND |= 0b00110000;}


#define RED_ON PORTD	|= 0b11000001
#define RED_OFF PORTD	&= ~0b11000001
#define RED_TOGGLE PIND |= 0b11000001

#define BUTTON_ONE_PRESS (!(PIND & (1<<1))) // -> PCINT17
#define BUTTON_TWO_PRESS (!(PINB & (1<<1))) // -> PCINT1

ISR(PCINT2_vect) // Triggers on PD changes
{
	GREEN_TOGGLE;
}


void init(void){
	
	
	// Pin change interrupt btn1
	sei();					// enable global interrupt
	PCICR |= (1<<PCIE2);	// enable PD PC interrupt (PCINT23..16)
	PCMSK2 |= (1<<PCINT17);	// enable PD1 interrupt
	
	// Buttons
	DDRD &= ~(1<<1);	//Button 1 PD1
	PORTD |= 1<<1;
	
	DDRB &= ~(1<<1);	//Button 2 PB1
	PORTB |= 1<<1;
	
	// LEDS
	DDRC |= 0b00111110;
	DDRD |= 0b11110001;

	// turn on green led
	GREEN_ON;
}

void check_buttons(void){
	/*if(BUTTON_ONE_PRESS)	{GREEN_ON;}
	else					{GREEN_OFF;}*/
	
	if(BUTTON_TWO_PRESS)	{RED_ON;}
	else					{RED_OFF;}
}

int main(void){
	init();
	while (1){
		check_buttons();
	}
}