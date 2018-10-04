
/*
The MIT License

Copyright (c) 2010 www.ooishoo.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
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
