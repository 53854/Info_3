#define LED_ON PORTB |= (1<<PORTB5)
#define LED_OFF PORTB &= ~(1<<PORTB)
#define LED_TOGGLE PINB |= (1<<PINB5)

#define F_CPU 16E6 // or F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>

int main(void){
	DDRB |= (1<< DDB5); // P5: Output
	LED_ON; // P5: On
	LED_TOGGLE; // P5: Toggle Output
	asm ("nop");
	
	while (1) {
		_delay_ms(1000);
		LED_TOGGLE; // P5: Toggle Output
	}
}