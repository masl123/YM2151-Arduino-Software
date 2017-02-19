
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


#include <midi_Settings.h>
#include <midi_Namespace.h>
#include <midi_Message.h>
#include <midi_Defs.h>


#include  <MIDI.h>
#include  <Arduino.h>
#include  <avr/pgmspace.h>
#include  <Wire.h>

#include "YM2151Driver.h" 
#include "NotePool.h"
#include "SysExHandler.h"
#include "PCHandler.h"
#include "CCHandler.h"
#include "EPROMManager.h"

	MIDI_CREATE_INSTANCE(HardwareSerial, Serial,     MIDI); //Set Midi In to RX1 on Arduino
//MIDI_CREATE_DEFAULT_INSTANCE();
	

	void setup() {

		
		YM2151.begin();
		YM2151.initLFO();
		MIDI.begin(MIDI_CHANNEL_OMNI); 
		

		//init
		EPROMManager.init();
		YM2151Driver.init();
		NotePool.init();
		CCHandler.init();
		PCHandler.init();
		SysExHandler.init();

		//set Handlers
		MIDI.setHandleSystemExclusive(handle_sysex);
		MIDI.setHandleNoteOn(handle_NoteOn);
		MIDI.setHandleNoteOff(handle_NoteOff);
		MIDI.setHandleProgramChange(handle_PC);
		MIDI.setHandleControlChange(handle_CC);
	} 

	void loop() {
		MIDI.read();
	}

	//SysEx:
	void handle_sysex(uint8_t* a, unsigned sizeofsysex){
		SysExHandler.handleSysEx(a, sizeofsysex);
		delete a; // we don't need the sysex Array anymore
	}

	void handle_CC(uint8_t channel, uint8_t number, uint8_t value){
		channel = channel - 1;
		CCHandler.handleCC(channel, number, value);
	}
	void handle_PC(uint8_t channel, uint8_t number){

	}

	void handle_NoteOn(uint8_t channel, uint8_t note, uint8_t value){
		channel = channel - 1;
		NotePool.handleNote(true, channel, note, value);
	}

	void handle_NoteOff(uint8_t channel, uint8_t note, uint8_t value){
		channel = channel - 1;
		NotePool.handleNote(false, channel, note, value);
	}


