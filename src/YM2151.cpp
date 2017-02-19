

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
	digitalWrite(P_IC, HIGH);
	digitalWrite(P_A0, HIGH);
	digitalWrite(P_WR, HIGH);
	digitalWrite(P_RD, HIGH);

	pinMode(P_A0, OUTPUT); 
	pinMode(P_WR, OUTPUT); 
	pinMode(P_RD, OUTPUT); 
	pinMode(P_IC, OUTPUT); 
	digitalWrite(P_IC, LOW);
	delay(100);
	digitalWrite(P_IC, HIGH);
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



PROGMEM const unsigned char KeyCodeTable[] = {
	0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x08, 0x09,
	0x0a, 0x0c, 0x0d, 0x0e, 0x10, 0x11, 0x12, 0x14,
	0x15, 0x16, 0x18, 0x19, 0x1a, 0x1c, 0x1d, 0x1e,
	0x20, 0x21, 0x22, 0x24, 0x25, 0x26, 0x28, 0x29,
	0x2a, 0x2c, 0x2d, 0x2e, 0x30, 0x31, 0x32, 0x34,
	0x35, 0x36, 0x38, 0x39, 0x3a, 0x3c, 0x3d, 0x3e,
	0x40, 0x41, 0x42, 0x44, 0x45, 0x46, 0x48, 0x49,
	0x4a, 0x4c, 0x4d, 0x4e, 0x50, 0x51, 0x52, 0x54,
	0x55, 0x56, 0x58, 0x59, 0x5a, 0x5c, 0x5d, 0x5e,
	0x60, 0x61, 0x62, 0x64, 0x65, 0x66, 0x68, 0x69,
	0x6a, 0x6c, 0x6d, 0x6e, 0x70, 0x71, 0x72, 0x74,
	0x75, 0x76, 0x78, 0x79, 0x7a, 0x7c, 0x7d, 0x7e,
};


/*!
\param ch
\param keycode
\param kf
*/
void YM2151_Class::setTone(uint8_t ch, uint8_t keycode, int16_t kf) {
	int16_t	offset_kf = (kf & 0x3f);
	int16_t	offset_note = keycode + (kf >> 6);
	if (offset_note < 0) offset_note = 0;
	if (offset_note > 0xbf) offset_note = 0xbf;

	YM2151.write(0x30 + ch, offset_kf << 2);
	YM2151.write(0x28 + ch, pgm_read_byte_near(KeyCodeTable + offset_note));
}

#endif







