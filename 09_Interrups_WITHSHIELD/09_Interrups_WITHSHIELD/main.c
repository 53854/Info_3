#include <avr/io.h>
#include <avr/interrupt.h>

#define GREEN_ON {PORTC |= 0b00111110; PORTD |= 0b00110000;}
#define GREEN_OFF {PORTC &= ~0b00111110; PORTD &= ~0b00110000;}
#define GREEN_TOGGLE {PORTC ^= 0b00111110; PORTD ^= 0b00110000;}


#define RED_ON {PORTD |= 0b11000000; PORTB |= (1<<0);}
#define RED_OFF {PORTD &= ~0b11000000; PORTB &= ~(1<<0);}
#define RED_TOGGLE {PORTD ^= 0b11000000; PORTB ^= (1<<0);}

#define BUTTON_ONE_PRESS (!(PIND & (1<<1))) // -> PCINT17
#define BUTTON_TWO_PRESS (!(PINB & (1<<1))) // -> PCINT1

ISR(PCINT2_vect) // Triggers on PD changes
{
	GREEN_TOGGLE;
}

ISR(PCINT0_vect) // Triggers on PD changes
{
	RED_TOGGLE;
}


void init(void){
	
	// DDR
	DDRB |= 0b00000001;
	DDRC |= 0b00111110;
	DDRD |= 0b11110001;
	
	// Buttons
	//DDRD &= ~(1<<1);	//Button 1 PD1
	PORTD |= 1<<1;
	
	//DDRB &= ~(1<<1);	//Button 2 PB1
	PORTB |= 1<<1;
	
	// turn on LEDS
	GREEN_ON;
	RED_ON;
	
	// Pin change interrupt btn1
	sei();					// enable global interrupt
	
	PCICR |= (1<<PCIE2);	// enable PD PC interrupt
	PCICR |= (1<<PCIE0);	// enable PB PC interrupt
	
	PCMSK0 |= (1<<1);	// mask out PB1 interrupt
	PCMSK2 |= (1<<1);	// mask out PD1 interrupt
}

int main(void){
	init();
	while (1){
		
	}
}