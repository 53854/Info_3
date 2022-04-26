#define F_CPU 16E6

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTON_ONE_PRESS ((PIND & (1<<1)))

static volatile uint16_t counter = 0;

void init(void){
	// LED
	DDRC |= 0b00111110; // PC1-6 as output
	DDRD |= 0b11110001; // PD0-4 as output
	
	PORTC	|= (1<<1);	// LED 1 one
	
	// Button
	DDRD &= ~(1<<1);	//Configure PD1 as Input
	PORTD |= 1<<1;		//Enable Internal Pull-Up at PD1
	
	// Interrupt
	/*sei();
	TIMSK0 |= (1<<OCIE0A);			// Timer0 A Match enable
	OCR0A = 155;
	
	TCCR0A |= (1<<WGM01);			// Configure CTC Mode
	TCCR0A &= ~(1<<WGM00);
	TCCR0B &= ~(1<<WGM02);
	
	TCCR0B |=(1<<CS02) | (1<<CS00); // Prescaler 1024
	TCCR0B &= ~(1<<CS01);*/
}

void onButtonPress(){
	//static volatile uint16_t oneSecondEqual = 60;
	
	counter = counter + 60;
	switch (counter){
		case 60:
		PORTC		&= ~(1<<1); // 1GN off
		PORTC		|= (1<<2); // 2GN on
		break;
		case 120:
		PORTC		&= ~(1<<2); // 2GN off
		PORTC		|= (1<<3); // 3GN on
		break;
		case 180:
		PORTC		&= ~(1<<3); // 3GN off
		PORTC		|= (1<<4); // 4GN on
		break;
		case 240:
		PORTC		&= ~(1<<4); // 4GN off
		PORTC		|= (1<<5); // 5GN on
		break;
		case 300:
		PORTC		&= ~(1<<5); // 5GN off
		PORTD		|= (1<<4); // 6GN on
		break;
		case 360:
		PORTD		&= ~(1<<4); // 6GN off
		PORTD		|= (1<<5); // 7GN on
		break;
		case 420:
		PORTD		&= ~(1<<5); // 7GN off
		PORTD		|= (1<<6); // 8RT on
		break;
		case 480:
		PORTD		&= ~(1<<6); // 8RT off
		PORTD		|= (1<<7); // 9RT on
		break;
		case 540:
		PORTD		&= ~(1<<7); // 9RT off
		PORTD		|= (1<<0); // 10RT on
		break;
		case 600:
		PORTD		&= ~(1<<0); // 10RT off
		PORTC		|= (1<<1); // 1GN on
		counter = 0;
		break;
	}
}

/*
ISR(TIMER0_COMPA_vect){
	
	//static volatile uint16_t oneSecondEqual = 60;
	static volatile uint16_t counter = 0;
	counter += 60;
	switch (counter){
		case 60:
		PORTC		&= ~(1<<1); // 1GN off
		PORTC		|= (1<<2); // 2GN on
		break;
		case 120:
		PORTC		&= ~(1<<2); // 2GN off
		PORTC		|= (1<<3); // 3GN on
		break;
		case 180:
		PORTC		&= ~(1<<3); // 3GN off
		PORTC		|= (1<<4); // 4GN on
		break;
		case 240:
		PORTC		&= ~(1<<4); // 4GN off
		PORTC		|= (1<<5); // 5GN on
		break;
		case 300:
		PORTC		&= ~(1<<5); // 5GN off
		PORTD		|= (1<<4); // 6GN on
		break;
		case 360:
		PORTD		&= ~(1<<4); // 6GN off
		PORTD		|= (1<<5); // 7GN on
		break;
		case 420:
		PORTD		&= ~(1<<5); // 7GN off
		PORTD		|= (1<<6); // 8RT on
		break;
		case 480:
		PORTD		&= ~(1<<6); // 8RT off
		PORTD		|= (1<<7); // 9RT on
		break;
		case 540:
		PORTD		&= ~(1<<7); // 9RT off
		PORTD		|= (1<<0); // 10RT on
		break;
		case 600:
		PORTD		&= ~(1<<0); // 10RT off
		PORTC		|= (1<<1); // 1GN on
		counter = 0;
		break;
	}

}
*/

int main(void){
	init();
	while (1){
		if(BUTTON_ONE_PRESS){
			onButtonPress();
			while(BUTTON_ONE_PRESS){ /*_delay_ms(100);*/}
		}
	}
}