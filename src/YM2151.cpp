

/*
	(c) Yukio Ishii' http://www.ooishoo.org/?page_id=15
*/



#include <avr/io.h>
#include <avr/pgmspace.h>

#include  "Common.h"
#include  "YM2151.h"

#define	DIRECT_IO

YM2151_Class	YM2151;

YM2151_Class::YM2151_Class()
{
}
void	YM2151_Class::begin()
{
	digitalWrite(PIN_IC, HIGH);
	digitalWrite(PIN_A0, HIGH);
	digitalWrite(PIN_WR, HIGH);
	digitalWrite(PIN_RD, HIGH);

	pinMode(PIN_A0, OUTPUT); 
	pinMode(PIN_WR, OUTPUT); 
	pinMode(PIN_RD, OUTPUT); 
	pinMode(PIN_IC, OUTPUT); 
	digitalWrite(PIN_IC, LOW);
	delay(100);
	digitalWrite(PIN_IC, HIGH);
	delay(100);
	return;
}

//
#define		RD_HIGH		(PORTB = PORTB | 0x4)
#define		RD_LOW		(PORTB = PORTB & ~0x4)
#define		WR_HIGH		(PORTB = PORTB | 0x8)
#define		WR_LOW		(PORTB = PORTB & ~0x8)
#define		A0_HIGH		(PORTB = PORTB | 0x10)
#define		A0_LOW		(PORTB = PORTB & ~0x10)

#define		BUS_READ	DDRD=0x02;DDRB=0x3c;
#define		BUS_WRITE	DDRD=0xfe;DDRB=0x3f;

#ifdef DIRECT_IO

static	uint8_t last_write_addr=0x00;

/*!
	\param addr		
	\param data		
 */
void	YM2151_Class::write(uint8_t addr,uint8_t data)
{
	uint8_t i,wi;
	volatile	uint8_t	*ddrD=&DDRD;
	volatile	uint8_t	*ddrB=&DDRB;
	volatile	uint8_t	*portD=&PORTD;
	volatile	uint8_t	*portB=&PORTB;
	
	if(last_write_addr != 0x20){
		*ddrD &= ~(_BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7));
		*ddrB &= ~(_BV(0) | _BV(1));
		wait(8);
		A0_LOW;
		wait(4);
		for(i=0;i<32;i++){
			RD_LOW;
			wait(4);
			if((*portB & _BV(1))==0){	// Read Status
				RD_HIGH;
				wait(4);
				break;
			}
			RD_HIGH;
			wait(8);
			if(i>16){
				wait(4);
			}
		}
	}
	wait(4);
	
	*ddrD |= (_BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7));
	*ddrB |= (_BV(0) | _BV(1));
	wait(4);
	A0_LOW;
	*portD = (addr << 2) | (*portD & 0x03);
	*portB = (addr >> 6) | (*portB & 0xfc);
	wait(4);
	WR_LOW;		// Write Address
	wait(4);
	WR_HIGH;
	wait(2);
	A0_HIGH;
	wait(2);
	*portD = (data << 2) | (*portD & 0x03);
	*portB = (data >> 6) | (*portB & 0xfc);
	
	wait(4);
	WR_LOW;		// Write Data
	wait(4);
	WR_HIGH;
	wait(2);
	last_write_addr = addr;
}

uint8_t	YM2151_Class::read()
{
	uint8_t i,wi,data;
	volatile	uint8_t	*ddrD=&DDRD;
	volatile	uint8_t	*ddrB=&DDRB;
	volatile	uint8_t	*portD=&PORTD;
	volatile	uint8_t	*portB=&PORTB;
	*ddrD &= ~(_BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7));
	*ddrB &= ~(_BV(0) | _BV(1));
	A0_HIGH;
	wait(4);
	RD_LOW;		// Read Data
	wait(4);
	data = 0;
	data |= (*portD)>>2;
	data |= (*portB)<<6;
	RD_HIGH;
	wait(4);
}

void	YM2151_Class::delayNanoseconds(unsigned int loop)
{
	uint8_t wi, tmp_lp;
        tmp_lp = loop/60;
	for(wi=0;wi<tmp_lp;wi++){
		// 16MHz  nop = @60nSec
		asm volatile("nop\n\t");
	}
}

void	YM2151_Class::wait(uint8_t loop)
{
	uint8_t wi;
	for(wi=0;wi<loop;wi++){
		// 16MHz  nop = @60nSec
		asm volatile("nop\n\t nop\n\t nop\n\t nop\n\t");
	}
}

/*! LFO
 */
void	YM2151_Class::initLFO()
{
	write(0x1,0x1);
}

#endif







