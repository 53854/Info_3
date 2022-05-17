/******************************************************************************
;*	Hochschule für Angewandte Wissenschaften Hamburg						  *
;*	Fakultät DMI															  *
;*	Department Medientechnik											 	  *
;*	Veranstaltung: Informatik  & Elektronik                                   *
;******************************************************************************
;*	TFT-Display per SPI-Schnittstelle										  *
;*	Das Display wird im 16Bit-Farbmodus im Hochformat um 180° gedreht be-     *
;*  trieben. Der Hintergrund wird mit dem Farbwert 0xFFE0 gefärbt.            *
;*  Ein rotes Quadrat aus 10x10 Pixeln wird mittig per Button1 und Button2    *
;*  bis zu den Diplayrändern flüssig bewegt.                                  *
;*  																		  *
;*	Versuch-Nr.: 5															  *
;******************************************************************************
;*	Namen/Matrikel-Nr.:  1. 2604332                                           *
;*              		 2. 2603380					     					  *
;******************************************************************************
;* 	Abgabedatum:             												  *
;******************************************************************************/

#define F_CPU 16E6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */
#define Pink            0xF81F

#define SPI_DDR DDRB
#define CS      PINB2
#define MOSI    PINB3
#define SCK     PINB5
#define D_C		PIND2		//display: Data/Command
#define Reset	PIND3		//display: Reset

#define BUTTON_ONE_PRESS ((PIND & (1<<1))) // -> PCINT17
#define BUTTON_TWO_PRESS ((PINB & (1<<1))) // -> PCINT1

// 10 * 10 Fenster x1=62,x2=71,y1=84,y2=93
uint16_t fenster[] = {0xEF08,0x1800, 0x123E, 0x1547,0x1354,0x165D};
uint8_t buttonOneStatus = 0; // 0 = PIND1 == 1
uint8_t buttonTwoStatus = 0; // 0 = PINB1 == 1

void init(){		
	sei();						// Interrupt
	TIMSK0 |= (1 << OCIE0A);    // Timer0 A Match enable
	OCR0A = 155;                // reset compare 10ms

	TCCR0A |= (1 << WGM01);     // Configure CTC Mode
	TCCR0A &= ~(1 << WGM00);
	TCCR0B &= ~(1 << WGM02);

	TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler 1024
	TCCR0B &= ~(1 << CS01);
	
	DDRD |= (1<<D_C)|(1<<Reset); //output: PD2 -> Data/Command; PD3 -> Reset
	
		// Buttons
		DDRD &= ~(1<<1);	//Configure PD1 as Input
		DDRB &= ~(1<<1);	//Configure PB1 as Input
		PORTD |= 1<<1;		//DDRD &= ~(1<<1);	//Button 1 PD1
		PORTB |= 1<<1;		//DDRB &= ~(1<<1);	//Button 2 PB1
}

void SPI_init(){
	//set CS, MOSI and SCK to output:
	SPI_DDR |= (1 << CS) | (1 << MOSI) | (1 << SCK);

	//enable SPI, set as master, and clock to fosc/4:
	SPCR = (1 << SPE) | (1 << MSTR);
}

void SPISend8Bit(uint8_t data){
	PORTB &= ~(1<<CS);				//CS low
	SPDR = data;					//load data into register
	while(!(SPSR & (1 << SPIF)));	//wait for transmission complete
	PORTB |= (1<<CS);				//CS high
}

void SendCommandSeq(const uint16_t * data, uint16_t Anzahl){
	uint32_t index;
	uint8_t SendeByte;
	for (index=0; index<Anzahl; index++){
		PORTD |= (1<<D_C);						//Data/Command auf High => Kommando-Modus
		SendeByte = (data[index] >> 8) & 0xFF;	//High-Byte des Kommandos
		SPISend8Bit(SendeByte);
		SendeByte = data[index] & 0xFF;			//Low-Byte des Kommandos
		SPISend8Bit(SendeByte);
		PORTD &= ~(1<<D_C);						//Data/Command auf Low => Daten-Modus
	}
}

void Display_init(void) {
	const uint16_t InitData[] ={
		//Initialisierungsdaten fuer 16Bit-Farben-Modus
		0xFDFD, 0xFDFD,
		/* pause */
		0xEF00, 0xEE04, 0x1B04, 0xFEFE, 0xFEFE,
		0xEF90, 0x4A04, 0x7F3F, 0xEE04, 0x4306,
		/* pause */
		0xEF90, 0x0983, 0x0800, 0x0BAF, 0x0A00,
		0x0500, 0x0600, 0x0700, 0xEF00, 0xEE0C,
		0xEF90, 0x0080, 0xEFB0, 0x4902, 0xEF00,
		0x7F01, 0xE181, 0xE202, 0xE276, 0xE183,
		0x8001, 0xEF90, 0x0000,
		/* pause */
		0xEF08, /*hochforat*/ 0x1800, 0x1200, 0x1583, 0x1300,
        0x16AF
	};
	
	_delay_ms(300);
	PORTD &= !(1<<Reset);			//Reset-Eingang des Displays auf Low => Beginn Hardware-Reset
	_delay_ms(75);
	PORTB |= (1<<CS);				//SSEL auf High
	_delay_ms(75);
	PORTD |= (1<<D_C);				//Data/Command auf High
	_delay_ms(75);
	PORTD |= (1<<Reset);			//Reset-Eingang des Displays auf High => Ende Hardware Reset
	_delay_ms(75);
	SendCommandSeq(&InitData[0], 2);
	_delay_ms(75);
	SendCommandSeq(&InitData[2], 10);
	_delay_ms(75);
	SendCommandSeq(&InitData[12], 23);
	_delay_ms(75);
	SendCommandSeq(&InitData[35], 6);
}

void send16BitColour(uint16_t colourhex){
	SPISend8Bit((colourhex >> 8) & 0xFF);	// High bit
	SPISend8Bit((colourhex) & 0xFF);		// Low bit
}

void fillScreen(uint16_t colourhex){
	for(uint16_t i = 0; i <= 132*176; i++){
		send16BitColour(colourhex);
	}
}

void fillWindow(uint16_t window, uint16_t colourhex){
	SendCommandSeq(window, 6);
	for(int i = 0; i <100; i++){
		send16BitColour(colourhex);		
	}
}

int main(void){
	init();
	SPI_init();
	Display_init();
	
	fillScreen(Yellow);
	
	fillWindow(fenster, Red);
	
	while(1){
		if(BUTTON_ONE_PRESS && buttonOneStatus == 0){
			buttonOneStatus = 1;
		}
		if(BUTTON_TWO_PRESS && buttonTwoStatus == 0){
			buttonTwoStatus = 1;
		}
	}
}

ISR(TIMER0_COMPA_vect){
	if(buttonOneStatus == 1 && !BUTTON_ONE_PRESS) {
		fillWindow(fenster,Yellow);
		fenster[2] += 1;
		fenster[3] += 1;
		fillWindow(fenster,Red);
		buttonOneStatus = 0;
	}
	if(buttonTwoStatus == 1 && !BUTTON_TWO_PRESS) {
		fillWindow(fenster,Yellow);
		fenster[2] -= 1;
		fenster[3] -= 1;
		fillWindow(fenster,Red);
		buttonTwoStatus = 0;
	}
}