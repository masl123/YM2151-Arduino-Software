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

#include "NotePool.h"
#include "YM2151Driver.h"


void NotePoolClass::init()
{
	NotePoolClass::notes = new uint8_t[8];
	for (byte i = 0; i < 8; i++){
		NotePoolClass::notes[i] = 0xFF;
	}
	NotePoolClass::resetNotes();
	mode = false;
}


void NotePoolClass::handleNote(bool on, uint8_t channel, uint8_t pitch, uint8_t velocity){
	
	if (NotePoolClass::mode){
		if (on){	
			NotePoolClass::notes[channel] = pitch;
			YM2151Driver.setTone(channel, pitch, 0);
			YM2151Driver.noteOff(channel);
			YM2151Driver.noteOn(channel);
		}
		else if (NotePoolClass::notes[channel] == pitch){
			NotePoolClass::notes[channel] = 0xFF;
			YM2151Driver.noteOff(channel);
		}
	}
	else{

		uint8_t ymChannel = 0xFF;
		if (on){

			
			ymChannel = findNote(pitch);
			if (ymChannel == 0xFF){

				byte ch = getFreeChannel();
				if (ch == 0xFF) {
					long r = random(0, 7); //just use a random Channel
					ch = (byte)r;
				}

				notes[ch] = pitch;
				YM2151Driver.setTone(ch, pitch, 0);
				YM2151Driver.noteOff(ch);
				YM2151Driver.noteOn(ch);
			}
		}
		else{
			ymChannel = findNote(pitch);
			if (ymChannel != 0xFF){
				notes[ymChannel] = 0xFF;
				YM2151Driver.noteOff(ymChannel);
			}
		}
	}
}

	/**
	*	false -> Poly
	*	true -> Channel
	*/
void NotePoolClass::setMode(bool channel){
	if (channel != mode){
		mode = channel;
		resetNotes();
	}
}

bool NotePoolClass::getMode(){
	return mode;
}


void NotePoolClass::resetNotes(){
	for (uint8_t i = 0; i < 8; i++){
		notes[i] = 0xFF;
		YM2151Driver.noteOff(i);
	}
}

uint8_t NotePoolClass::findNote(uint8_t note){
	for (uint8_t i = 0; i < 8; i++){
		if (notes[i] == note){
			return i;
		}
	}
	return 0xFF;
}

uint8_t NotePoolClass::getFreeChannel(){
	for (uint8_t i = 0; i < 8; i++){
		if (notes[i] == 0xFF){
			return i;
		}
	}
	return 0xFF;
}


NotePoolClass NotePool;

