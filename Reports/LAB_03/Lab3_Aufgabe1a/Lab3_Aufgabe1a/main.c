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
;*	Namen/Matrikel-Nr.:  1.		                                              *
;*              		 2.									     			  *
;******************************************************************************
;* 	Abgabedatum:             												  *
;******************************************************************************/

#define F_CPU 16E6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SPI_DDR DDRB
#define CS      PINB2
#define MOSI    PINB3
#define SCK     PINB5
#define D_C		PIND2		//display: Data/Command
#define Reset	PIND3		//display: Reset

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
		0x8001, 0xEF90, 0x0000
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
}



int main(void){
	DDRD |= (1<<D_C)|(1<<Reset);		//output: PD2 -> Data/Command; PD3 -> Reset
	SPI_init();
	Display_init();
	while(1){;}
}
