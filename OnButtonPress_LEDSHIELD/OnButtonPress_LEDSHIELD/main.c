#include <avr/io.h>

#define LED_ON PORTC |= (1<<1)
#define LED_OFF PORTC &= ~(1<<1)
#define LED_TOGGLE PINC |= (1<<1)
#define BUTTON_PRESS (!(PIND & (1<<1)))

void init(void){
	// LED
	DDRC |= (1<<1);// Configure PC1 as Output
	LED_ON;
	
	// Button
	DDRD &= ~(1<<1);	//Configure PD1 as Input
	PORTD |= 1<<1;		//Enable Internal Pull-Up at PD1
}

void check_button(void){
	if(BUTTON_PRESS)
	LED_ON;
	else
	LED_OFF;
}

int main(void){
	init();
	while (1){
		check_button();
	}
}