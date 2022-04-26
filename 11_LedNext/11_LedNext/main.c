#define F_CPU 16E6

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTON_ONE_PRESS (!(PIND & (1<<1)))

static volatile uint16_t counter = 0;


void init(void){
	// LED
	DDRB |= 0b00000001; // PB0 as output
	DDRC |= 0b00111110; // PC1-6 as output
	DDRD |= 0b11110001; // PD0-4 as output
	
	PORTC	|= (1<<1);	// LED 1 one
	
	// Button
	DDRD &= ~(1<<1);	//Configure PD1 as Input
	PORTD |= 1<<1;		//Enable Internal Pull-Up at PD1
	
	
	// Interrupt
	sei();
	TIMSK0 |= (1 << OCIE0A);    // Timer0 A Match enable
	OCR0A = 155;                // reset compare 10ms

	TCCR0A |= (1 << WGM01);     // Configure CTC Mode
	TCCR0A &= ~(1 << WGM00);
	TCCR0B &= ~(1 << WGM02);

	TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler 1024
	TCCR0B &= ~(1 << CS01);
}


static volatile uint8_t buttonStatus = 0; // 0 = PIND1 == 1

ISR(TIMER0_COMPA_vect){
	//int i = buttonStatus ^ ~PIND1;
	
	if(buttonStatus == 1 && !BUTTON_ONE_PRESS) {
		buttonStatus = 0;
		counter += 100;
		switchLED();
	}
}

void switchLED(){
	//static volatile uint16_t oneSecondEqual = 60;
	switch (counter){
		case 100:
		PORTC		&= ~(1<<1); // 1GN off
		PORTC		|= (1<<2); // 2GN on
		break;
		case 200:
		PORTC		&= ~(1<<2); // 2GN off
		PORTC		|= (1<<3); // 3GN on
		break;
		case 300:
		PORTC		&= ~(1<<3); // 3GN off
		PORTC		|= (1<<4); // 4GN on
		break;
		case 400:
		PORTC		&= ~(1<<4); // 4GN off
		PORTC		|= (1<<5); // 5GN on
		break;
		case 500:
		PORTC		&= ~(1<<5); // 5GN off
		PORTD		|= (1<<4); // 6GN on
		break;
		case 600:
		PORTD		&= ~(1<<4); // 6GN off
		PORTD		|= (1<<5); // 7GN on
		break;
		case 700:
		PORTD		&= ~(1<<5); // 7GN off
		PORTD		|= (1<<6); // 8RT on
		break;
		case 800:
		PORTD		&= ~(1<<6); // 8RT off
		PORTD		|= (1<<7); // 9RT on
		break;
		case 900:
		PORTD		&= ~(1<<7); // 9RT off
		PORTB		|= (1<<0); // 10RT on
		break;
		case 1000:
		PORTB		&= ~(1<<0); // 10RT off
		PORTC		|= (1<<1); // 1GN on
		counter = 0;
		break;
	}
}

int main(void){
	init();
	
	while (1){
		if(BUTTON_ONE_PRESS && buttonStatus == 0){
			buttonStatus = 1;
		}
	}
}