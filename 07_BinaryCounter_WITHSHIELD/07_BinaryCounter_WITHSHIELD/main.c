#include <avr/io.h>
#include <util/delay.h>


#define F_CPU 16E6			// or F_CPU 16000000

#define BUTTON_ONE_PRESS (!(PIND & (1<<1)))
#define BUTTON_TWO_PRESS (!(PINB & (1<<1)))

unsigned counter = 0;


void init(void){
	// LED
	DDRC |= 0b00011110;		// Configure PB5 as Output
	PORTC &= ~0b00011110;
	
	// Buttons
	PORTB |= 1<<1;		//Enable Internal Pull-Up at PB1
	
	DDRD &= ~(1<<1);	//Configure PD1 as Input
	PORTD |= 1<<1;		//Enable Internal Pull-Up at PD1
}

void displayBinaryCount(unsigned c){

	PORTC |= (1 << c); 
	

	/*int i = 1;
	int n = counter;
	while (n > 0) {
		
		if((n%2) == 1)	PORTC |= (1<<i);
		else		PORTC &= ~(1<<i);
	
		n = n / 2;
		i++;
	}*/

}


int main(void){
	init();
	
	while (1){
 		if(BUTTON_ONE_PRESS){
			counter ++;
			PORTC &= ~0b11111111;
			PORTC |= (1 << (counter%16)); 
		} else if (BUTTON_TWO_PRESS){
			counter = 0;
		}
		

	}
}