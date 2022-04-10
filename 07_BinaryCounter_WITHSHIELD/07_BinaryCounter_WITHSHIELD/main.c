#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>


#define F_CPU 16E6			// or F_CPU 16000000
#define stdout	

#define BUTTON_ONE_PRESS ((PIND & (1<<1)))
#define BUTTON_TWO_PRESS ((PINB & (1<<1)))

unsigned counter = 0;
//unsigned binArr[4];

void init(void){
	// LED
	DDRB |= 0b00011110;		// Configure PB5 as Output
	
	// Buttons
	DDRB &= ~(1<<1);	//Configure PB1 as Input
	PORTB |= 1<<1;		//Enable Internal Pull-Up at PB1
	
	DDRD &= ~(1<<1);	//Configure PD1 as Input
	PORTD |= 1<<1;		//Enable Internal Pull-Up at PD1
}

void displayBinaryCount(){

	int i = 1;
	int n = counter;
	while (n > 0) {
		
		if(n%2!=0){
			PORTC |= (1<<i);
		}
		else{PORTC &= ~(1<<i);}
		//binArr[i] = n%2;
		n = n / 2;
		i++;
	}

}


int main(void){
	init();
	
	while (1){
 		if(BUTTON_ONE_PRESS){
			counter ++;
			counter %= 16;
			displayBinaryCount(counter);
			} else if (BUTTON_TWO_PRESS){
			counter = 0;
		}
		

	}
}