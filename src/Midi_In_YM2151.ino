
	/**
	*	YM2151 - Chip Controller Software for ARDUINO
	*	(C) 2016  Marcel Weiﬂ
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


	MIDI_CREATE_INSTANCE(HardwareSerial, Serial,     MIDI); //Set Midi In to RX1 on Arduino
//MIDI_CREATE_DEFAULT_INSTANCE();
	extern  PROGMEM const unsigned char SampleTimbre01[];

	void setup() {
		//init
		
		
		YM2151Driver.init();
		NotePool.init();
		CCHandler.init();
		PCHandler.init();
		SysExHandler.init();
		
		
		YM2151.begin();
		YM2151.initLFO();
		MIDI.begin(MIDI_CHANNEL_OMNI); 

    
		for (byte i = 0; i < 8; i++){
			YM2151Driver.loadTimbre(i, (uint16_t)SampleTimbre01);
			YM2151Driver.setVolume(i, 0xf, 0);
			YM2151Driver.setPan(i, 0x40);
		}
		YM2151.write(0x19,255); 

		YM2151.initLFO();

		

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


	//Default Patch
	PROGMEM const unsigned char SampleTimbre01[] = {
		// ToneNo FLCON SlotMask
		0x01, 0x3e, 0x0f,
		// DT1_MUL
		0x36, 0x11, 0x72, 0x23,
		// TL
		0x18, 0x03, 0x03, 0x03,
		// KS_AR
		0x1f, 0x14, 0x14, 0x14,
		// AME_D1R
		0x14, 0x82, 0x82, 0x82,
		// DT2_D2R
		0x00, 0x01, 0x01, 0x01,
		// D2R_RR
		0x0a, 0x3a, 0x3a, 0x3a
	};
