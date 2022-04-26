#define F_CPU 16E6

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_ON PORTC |= (1 << 1)
#define LED_OFF PORTC &= ~(1 << 1)
#define LED_TOGGLE PORTC ^= (1 << 1)

void init(void)
{
    // LED
    DDRB |= 0b00000001;
    DDRC |= 0b00111110; // PC1-6 as output
    DDRD |= 0b11110001; // PD0-4 as output

    PORTC |= (1 << 1); // LED 1 one

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

// timer 1 statt 0 da timer 1 16bit statt 8bit
ISR(TIMER0_COMPA_vect)
{
    static volatile uint16_t counter = 0;
    counter++;
    switch (counter)
    {
    // 0ms = C1 = 1GN ON
    case 100:               // 100 * 10ms = 1 second passed = 2GN on
        PORTC &= ~(1 << 1); // 1GN off
        PORTC |= (1 << 2);  // 2GN on
        break;
    case 200:               // 200 * 10ms = 2 seconds passed = 3GN on
        PORTC &= ~(1 << 2); // 2GN off
        PORTC |= (1 << 3);  // 3GN on
        break;
    case 300:               // 300 * 10ms = 3 seconds passed = ....
        PORTC &= ~(1 << 3); // 3GN off
        PORTC |= (1 << 4);  // 4GN on
        break;
    case 400:               // 400 * 10ms = 4......
        PORTC &= ~(1 << 4); // 4GN off
        PORTC |= (1 << 5);  // 5GN on
        break;
    case 500:               // 500 * .......
        PORTC &= ~(1 << 5); // 5GN off
        PORTD |= (1 << 4);  // 6GN on
        break;
    case 600:
        PORTD &= ~(1 << 4); // 6GN off
        PORTD |= (1 << 5);  // 7GN on
        break;
    case 700:
        PORTD &= ~(1 << 5); // 7GN off
        PORTD |= (1 << 6);  // 8RT on
        break;
    case 800:
        PORTD &= ~(1 << 6); // 8RT off
        PORTD |= (1 << 7);  // 9RT on
        break;
    case 900:
        PORTD &= ~(1 << 7);  // 9RT off
        PORTB |= 0b00000001; //|= (1<<0); // 10RT on
        break;
    case 1000:
        PORTB &= ~0b00000001; //&= ~(1<<0); // 10RT off
        PORTC |= (1 << 1);    // 1GN on
        counter = 0;
        break;
    }
}

int main(void)
{
    init();
    while (1)
    {
    }
}