/**
*	YM2151 - Chip Controller Software for ARDUINO
*	(C) 2016  Marcel Weiß
*
*	This program is free software : you can redistribute it and / or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _CCHANDLER_h
#define _CCHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class CCHandlerClass
{
protected:
	



 public:
	void init();
	void handleCC(uint8_t channel, uint8_t number, uint8_t value);
	void handleLFO(uint8_t number, uint8_t value);
	void handleNoise(uint8_t number, uint8_t value);
	void handleCommon(uint8_t channel, uint8_t number, uint8_t value);
	void handleOp(uint8_t channel, uint8_t op, uint8_t number, uint8_t value);
};

extern CCHandlerClass CCHandler;

#endif

