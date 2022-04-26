#define F_CPU 16E6

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTON_ONE_PRESS ((PIND & (1 << 1)))

static volatile uint16_t counter = 0;

void init(void)
{
	// LED
	DDRB |= 0b00000001; // PB0 as output
	DDRC |= 0b00111110; // PC1-6 as output
	DDRD |= 0b11110001; // PD0-4 as output

	PORTC |= (1 << 1); // LED 1 one

	// Button
	DDRD &= ~(1 << 1); // Configure PD1 as Input
	PORTD |= 1 << 1;   // Enable Internal Pull-Up at PD1
}

void onButtonPress()
{
	// static volatile uint16_t oneSecondEqual = 60;
	switch (counter)
	{
	case 100:
		PORTC &= ~(1 << 1); // 1GN off
		PORTC |= (1 << 2);	// 2GN on
		break;
	case 200:
		PORTC &= ~(1 << 2); // 2GN off
		PORTC |= (1 << 3);	// 3GN on
		break;
	case 300:
		PORTC &= ~(1 << 3); // 3GN off
		PORTC |= (1 << 4);	// 4GN on
		break;
	case 400:
		PORTC &= ~(1 << 4); // 4GN off
		PORTC |= (1 << 5);	// 5GN on
		break;
	case 500:
		PORTC &= ~(1 << 5); // 5GN off
		PORTD |= (1 << 4);	// 6GN on
		break;
	case 600:
		PORTD &= ~(1 << 4); // 6GN off
		PORTD |= (1 << 5);	// 7GN on
		break;
	case 700:
		PORTD &= ~(1 << 5); // 7GN off
		PORTD |= (1 << 6);	// 8RT on
		break;
	case 800:
		PORTD &= ~(1 << 6); // 8RT off
		PORTD |= (1 << 7);	// 9RT on
		break;
	case 900:
		PORTD &= ~(1 << 7); // 9RT off
		PORTB |= (1 << 0);	// 10RT on
		break;
	case 1000:
		PORTB &= ~(1 << 0); // 10RT off
		PORTC |= (1 << 1);	// 1GN on
		counter = 0;
		break;
	}
}

int main(void)
{
	init();

	while (1)
	{
		if (BUTTON_ONE_PRESS)
		{
			counter += 100;
			onButtonPress();
			while (BUTTON_ONE_PRESS)
			{
				_delay_ms(100);
			}
		}
	}
}