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

#ifndef _NOTEPOOL_h
#define _NOTEPOOL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "YM2151.h"





class NotePoolClass
{
 private:
		uint8_t* notes;
		bool mode;


		void resetNotes();
		uint8_t findNote(uint8_t note);
		uint8_t getFreeChannel();

 public: void init();
		 void handleNote(bool on, uint8_t channel, uint8_t pitch, uint8_t velocity);
		 void setMode(bool channel);
		 bool getMode();
};

extern NotePoolClass NotePool;

#endif

