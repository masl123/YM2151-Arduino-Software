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


// 
// Class to Handle Midi CC incomming
// 


/*
* --------------------
* MIDI CC:			  |
* --------------------
*
* OPERATOR 1: 16 - 25
* 16	Level
* 17	Mul
* 18   Det1
* 19   Det2
* 20	KSR
* 21	At (Rate)
* 22   Dec1 (Rate)
* 23   Dec1 (Level)
* 24   Dec2 (Rate)
* 25   Rel (Rate)
*
* OPERATOR 2: 26 - 35
* 26	Level
* 27	Mul
* 28   Det1
* 29   Det2
* 30	KSR
* 31	At (R)
* 32   Dec1 (R)
* 33   Dec1 (L)
* 34   Dec2 (R)
* 35   Rel (R)
*
* OPERATOR 3: 36 - 45
* 36	Level
* 37	Mul
* 38   Det1
* 39   Det2
* 40	KSR
* 41	At (R)
* 42   Dec1 (R)
* 43   Dec1 (L)
* 44   Dec2 (R)
* 45   Rel (R)
*
* OPERATOR 4: 46 - 55
* 46	Level
* 47	Mul
* 48   Det1
* 49   Det2
* 50	KSR
* 51	At (R)
* 52   Dec1 (R)
* 53   Dec1 (L)
* 54   Dec2 (R)
* 55   Rel (R)
*
* COMMON: 56 - 67
* 56	Op #1 - ON/OFF		
* 57	Op #2 - ON/OFF
* 58	Op #3 - ON/OFF
* 59	Op #4 - ON/OFF
* 60	AMSense Op #1 - ON/OFF
* 61	AMSense Op #2 - ON/OFF
* 62	AMSense Op #3 - ON/OFF
* 63	AMSense Op #4 - ON/OFF
* 64	FB
* 65	AMS
* 66	PMS
* 67	Algorithm
*
* LFO 68 - 71
* 68	Freq
* 69	LFO Wave
* 70	AMP Mod Depth
* 71	PHASE Mod Depth
*
* Noise 72-74
*
* 72	Noise Freq
* 73	Noise Enable
*
* OTHER:
* 74	POLY/CHANNEL Mode (0 = CHANNEL; 127 = POLY)
* 
* */


#include "CCHandler.h"
#include "YM2151Driver.h"
#include "NotePool.h"

void CCHandlerClass::init()
{


}


void CCHandlerClass::handleCC(uint8_t channel, uint8_t number, uint8_t value){

		//TODO Handle Poly Mode
		if (number == 74){
			NotePool.setMode(value == 0);
		}

		if (number < 56 && number >= 16){
			if (NotePool.getMode()){
				handleOp(channel & 0x07, (number - 16) / 10, (number - 16) % 10, value & 0x7f);
			}
			else{
				for (byte i = 0; i < 8; i++){
					handleOp(i & 0x07, (number - 16) / 10, (number - 16) % 10, value & 0x7f);
				}
			}	
		}
		
		else if (number <= 67 && number >= 56) {
			if (NotePool.getMode()) {
				handleCommon(channel, number, value);
			}
			else {
				for (byte i = 0; i < 8; i++) {
					handleCommon(i, number, value);
				}
			}
		}else if (number >= 68 && number <= 71) {
			handleLFO(number, value);
		}
		else if (number <= 73 && number >= 72) {
			handleNoise(number, value);
		}
}


void CCHandlerClass::handleLFO(uint8_t number , uint8_t value) {
	switch (number){
		case 68:
			YM2151Driver.setLFOFreq(value);
			break;
		case 69:
			YM2151Driver.setWaveForm(value);
			break;
		case 70: 
			YM2151Driver.setAmpDepth(value);
			
			break;
		case 71: 
			YM2151Driver.setPhaseDepth(value);
			break;
		default:
			break;
	}
}

void CCHandlerClass::handleNoise(uint8_t number, uint8_t value) {
	switch (number) {
		case 72:
			YM2151Driver.setNoiseFreq(value);
			break;
		case 73:
			YM2151Driver.setNoiseEnable(value);
			break;
		default:
			break;
	}
}

void CCHandlerClass::handleCommon(uint8_t channel, uint8_t number, uint8_t value) {
	switch (number) {
		case 56:
			YM2151Driver.setOpActive(channel, 0, value);
			break;
		case 57:
			YM2151Driver.setOpActive(channel, 1, value);
			break;
		case 58:
			YM2151Driver.setOpActive(channel, 2, value);
			break;
		case 59:
			YM2151Driver.setOpActive(channel, 3, value);
			break;
		case 60:
			YM2151Driver.setAMSenseEn(channel, 0, value);
			break;
		case 61:
			YM2151Driver.setAMSenseEn(channel, 1, value);
			break;
		case 62:
			YM2151Driver.setAMSenseEn(channel, 2, value);
			break;
		case 63:
			YM2151Driver.setAMSenseEn(channel, 3, value);
			break;
		case 64:
			YM2151Driver.setFeedback(channel, value);
			break;
		case 65:
			YM2151Driver.setAMSense(channel, value);
			break;
		case 66:
			YM2151Driver.setPMSense(channel, value);
			break;
		case 67:
			YM2151Driver.setAlgorithm(channel, value);
			break;
		default:
			break;
	}
}



void CCHandlerClass::handleOp(uint8_t channel, uint8_t op, uint8_t number, uint8_t value){
	//op*8+channel
	switch (number){
		case 0: 
			//OP Volume
			YM2151Driver.setOpVolume(channel, op, value);
			break;
		case 1:  
			//mul
			YM2151Driver.setMul(channel, op, value);
			break;
		case 2: 
			//Det1
			YM2151Driver.setDet1(channel, op, value);
			break;
		case 3: 
			//det2
			YM2151Driver.setDet2(channel, op, value);
			break;
		case 4: 
			//KSR
			YM2151Driver.setKSR(channel, op, value);
			break;
		case 5: 
			//AT R
			YM2151Driver.setATR(channel, op, value);
			break;
		case 6: 
			//Dec1(R)
			YM2151Driver.setDec1R(channel, op, value);
			break;
		case 7: 
			//Dec1(L)
			YM2151Driver.setDec1L(channel, op, value);
			break;
		case 8: 
			//Dec2(R)
			YM2151Driver.setDec2R(channel, op, value);
			break;
		case 9: 
			//Rel(R)
			YM2151Driver.setRel(channel, op, value);
			break;
		default: break;
	}









}

CCHandlerClass CCHandler;

