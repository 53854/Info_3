#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_ON			PORTB |= (1<<5)
#define LED_OFF			PORTB &= ~(1<<5)
#define LED_TOGGLE		PINB |= (1<<5)
#define BUTTON_PRESS	(!(PINB & (1<<7)))

ISR(PCINT0_vect)
{
	LED_TOGGLE;
}

void init(void){
	// Button
	DDRB &= ~(1<<7);	//Configure PB1 as Input
	PORTB |= (1<<7);	//Enable Internal Pull-Up

	// LED
	DDRB |= (1<<5);		// Configure PB5 as Output
	LED_ON;

	//Interrupt
	sei();					//enable global interrupt
	PCICR |= (1<<PCIE0);	// enable PB PC interrupt
	PCMSK0 |= (1<<PCINT7);	// enable PB7 interrupt
}

int main(void)
{
	init();
	while (1){
		// Nothing to do
	}
}