
/*
	(c) Yukio Ishii' http://www.ooishoo.org/?page_id=15
*/



#if !defined( YM2151_H_INCLUDED )
#define YM2151_H_INCLUDED
#include	"arduino.h"
#include	<avr/pgmspace.h>

class	YM2151_Class{
	public:
		YM2151_Class();
		void	begin();
		void	initLFO();
		void setTone(uint8_t ch, uint8_t keycode, int16_t kf);
		uint8_t	read();
		void	write(uint8_t addr,uint8_t data);
		
private:

		static const uint8_t P_D0 = 2;
		static const uint8_t P_D1 = 3;
		static const uint8_t P_D2 = 4;
		static const uint8_t P_D3 = 5;
		static const uint8_t P_D4 = 6;
		static const uint8_t P_D5 = 7;
		static const uint8_t P_D6 = 8;
		static const uint8_t P_D7 = 9;
		
		static const uint8_t P_RD = 10;
		static const uint8_t P_WR = 11;
		static const uint8_t P_A0 = 12;
		static const uint8_t P_IC = 13;
		
		void	wait(uint8_t loop);
		void	delayNanoseconds(unsigned int loop);
};
extern YM2151_Class YM2151;
#endif  //YM2151H_INCLUDED
