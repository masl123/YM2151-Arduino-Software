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



//TODO Use Most Significant Bits instead of LSBs for Values (Top part of byte instead of bottom part) e.g.
//
//	((value & 0xf) << 4       -->   ((value & 0x78) << 1)
//	((value & 0x3) << 6)	  -->	((value & 0x60) << 1)
//	(value & 0x0F)			  -->	((value & 0x78) >> 3)
//
// ... DONE, but needs Testing



#include "YM2151Driver.h"
#include "EPROMManager.h"



void YM2151DriverClass::init()
{
	YM2151Driver.loadInitPatch();
	for (byte i = 0; i < 8; i++) {
		YM2151Driver.setPan(i, 0x40);
	}
	YM2151.initLFO();

	setMasterTune(*EPROMManager.load(0x00, 1));
}

void YM2151DriverClass::setOpVolume(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	TotalLevel[adr] = (TotalLevel[adr] & 0x80) | (value & 0x7F);
	YM2151.write(0x60 + adr, TotalLevel[adr]);
}
void YM2151DriverClass::setMul(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	DetPhase_Mul[adr] = (DetPhase_Mul[adr] & 0xf0) | ((value & 0x78) >> 3);
	YM2151.write(0x40 + adr, DetPhase_Mul[adr]);
}
void YM2151DriverClass::setDet1(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	DetPhase_Mul[adr] = (DetPhase_Mul[adr] & 0x0f) | ((value & 0x70));
	YM2151.write(0x40 + adr, DetPhase_Mul[adr]);
}
void YM2151DriverClass::setDet2(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Decay2[adr] = (EG_Decay2[adr] & 0x3F) | ((value & 0x60) << 1);
	YM2151.write(0xC0 + adr, EG_Decay2[adr]);
}
void YM2151DriverClass::setKSR(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Attack[adr] = (EG_Attack[adr] & 0x3F) | ((value & 0x60) << 1);
	YM2151.write(0x80 + adr, EG_Attack[adr]);
}
void YM2151DriverClass::setATR(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Attack[adr] = (EG_Attack[adr] & 0xC0) | ((value & 0x7C) >> 2);
	YM2151.write(0x80 + adr, EG_Attack[adr]);
}
void YM2151DriverClass::setDec1R(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Decay1[adr] = (EG_Decay1[adr] & 0x80) | ((value & 0x7C) >> 2);
	YM2151.write(0xA0 + adr, EG_Decay1[adr]);
}

void YM2151DriverClass::setAMSenseEn(uint8_t channel, uint8_t op, uint8_t value) {
	uint8_t adr = getAdr(channel, op);
	EG_Decay1[adr] = (EG_Decay1[adr] & 0x7F) | ((value & 0x40) << 1);
	YM2151.write(0xA0 + adr, EG_Decay1[adr]);
}

void YM2151DriverClass::setDec1L(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EGDec_RelRate[adr] = (EGDec_RelRate[adr] & 0x0f) | ((value & 0x78) << 1);
	YM2151.write(0xE0 + adr, EGDec_RelRate[adr]);
}
void YM2151DriverClass::setDec2R(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Decay2[adr] = (EG_Decay2[adr] & 0xf0) | ((value & 0x78) >> 3);
	YM2151.write(0xC0 + adr, EG_Decay2[adr]);
}
void YM2151DriverClass::setRel(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	//uint8_t fix = (EGDec_RelRate[adr] & 0xf0) | (value & 0x0F);
	EGDec_RelRate[adr] = (EGDec_RelRate[adr] & 0xf0) | ((value & 0x78) >> 3);
	YM2151.write(0xE0 + adr, EGDec_RelRate[adr]);
}


void YM2151DriverClass::setOpActive(uint8_t channel, uint8_t op, uint8_t value) {

	bool value2 = value >= 64;

	//Switch the Values because the sequence (M1,M2,C1,C2) is Wrong in Register #0x08, but right for all other Registers
	if (op == 1) {
		op = 2;
	}else if (op == 2) {
		op = 1;
	}

	if (value2) {
		OpOn[channel] = OpOn[channel] | (1 << op);
	}
	else {
		OpOn[channel] = OpOn[channel] & (~(1 << op));
	}
}


void YM2151DriverClass::setAlgorithm(uint8_t channel, uint8_t value){
	ChannelControl[channel] = (ChannelControl[channel] & 0xf8) | ((value & 0x70)>>4);
	YM2151.write(0x20 + channel, ChannelControl[channel]);
}


void YM2151DriverClass::setFeedback(uint8_t channel, uint8_t value){
	ChannelControl[channel] = (ChannelControl[channel] & 0xC7) | ((value & 0x70) >> 1);
	YM2151.write(0x20 + channel, ChannelControl[channel]);
}




void YM2151DriverClass::setPMSense(uint8_t channel, uint8_t value) {
	PhAmp_ModSens[channel] = (PhAmp_ModSens[channel] & 0x03) | ((value & 0x70));
	YM2151.write(0x38 + channel, PhAmp_ModSens[channel]);
}


void YM2151DriverClass::setAMSense(uint8_t channel, uint8_t value) {
	PhAmp_ModSens[channel] = (PhAmp_ModSens[channel] & 0x70) | ((value & 0x60) >> 5);
	YM2151.write(0x38 + channel, PhAmp_ModSens[channel]);
}



void YM2151DriverClass::setLFOFreq(uint8_t value) {
	LFOFreq = value << 1;
	YM2151.write(0x18, LFOFreq);
}


void YM2151DriverClass::setWaveForm(uint8_t value) {
	CtrlOut_WF = (CtrlOut_WF & 0xC0) | ((value & 0x60) >> 5);
	YM2151.write(0x1B, CtrlOut_WF);
}


void YM2151DriverClass::setCTRLout(uint8_t value) {
	CtrlOut_WF = (CtrlOut_WF & 0x03) | ((value & 0x60) << 1);
	YM2151.write(0x1B, CtrlOut_WF);
}


void YM2151DriverClass::setPhaseDepth(uint8_t value) {
	PhAmpMod = ((value)) | 0x80;
	YM2151.write(0x19, PhAmpMod);
}


void YM2151DriverClass::setAmpDepth(uint8_t value) {
	PhAmpMod = ((value)) & 0x7F;
	YM2151.write(0x19, PhAmpMod);
}


void YM2151DriverClass::setNoiseEnable(uint8_t value) {
	Noize = (Noize & 0x1F) | ((value>=64)<<7);
	YM2151.write(0x0F, Noize);
}


void YM2151DriverClass::setNoiseFreq(uint8_t value) {
	Noize = (Noize & 0x80) | ((value & 0x7C) >> 2);
	YM2151.write(0x0F, Noize);
}


void YM2151DriverClass::setPan(uint8_t channel, uint8_t value){
	
	//rethink about that (first switches on left, then right, then both, BUT should be left,both,right or so)
	byte pan[] = { 0x00, 0x01, 0x03, 0x02 };
	ChannelControl[channel] = (ChannelControl[channel] & 0x3F) | (pan[(value & 0x60) >> 5] << 6);
	YM2151.write(0x20 + channel, ChannelControl[channel]);
}


void YM2151DriverClass::noteOn(uint8_t channel){
	YM2151.write(0x08, (OpOn[channel]<<3) | (channel & 0x7));
}


void YM2151DriverClass::noteOff(uint8_t channel){
	YM2151.write(0x08, 0x00 | (channel & 0x7));
}

void YM2151DriverClass::setTone(uint8_t ch, uint8_t keycode, int16_t kf)
{
	YM2151.setTone(ch, keycode, kf + (MasterTune - 63));
}

void YM2151DriverClass::setMasterTune(uint8_t value)
{
	if (value != *EPROMManager.load(0x00, 1)) {
		EPROMManager.save(0x00, &value, 1);
	}

	this->MasterTune = value;
}

extern  PROGMEM const unsigned char initPatch[];

void YM2151DriverClass::loadInitPatch() {
	//LFO
	setLFOFreq(pgm_read_byte_near(initPatch));
	setAmpDepth(pgm_read_byte_near(initPatch + 1));
	setPhaseDepth(pgm_read_byte_near(initPatch + 2));
	setWaveForm(pgm_read_byte_near(initPatch + 3));
	setNoiseFreq(pgm_read_byte_near(initPatch + 4));
	setNoiseEnable(pgm_read_byte_near(initPatch + 11));


	for (int i = 0; i < 8; i++) {
		//CH
		setPan(i, pgm_read_byte_near(initPatch + 5));
		setFeedback(i, pgm_read_byte_near(initPatch + 6));
		setAlgorithm(i, pgm_read_byte_near(initPatch + 7));
		setAMSense(i, pgm_read_byte_near(initPatch + 8));
		setPMSense(i, pgm_read_byte_near(initPatch + 9));
		byte a = pgm_read_byte_near(initPatch + 10);
		setOpActive(i, 0, ((a >> 3) & 0x1) << 6);
		setOpActive(i, 2, ((a >> 4) & 0x1) << 6);
		setOpActive(i, 1, ((a >> 5) & 0x1) << 6);
		setOpActive(i, 3, ((a >> 6) & 0x1) << 6);

		//OPs
		for (int a = 0; a < 4; a++) {
			setATR(i, a, pgm_read_byte_near(initPatch + 12));
			setDec1R(i, a, pgm_read_byte_near(initPatch + 13));
			setDec2R(i, a, pgm_read_byte_near(initPatch + 14));
			setRel(i, a, pgm_read_byte_near(initPatch + 15));
			setDec1L(i, a, pgm_read_byte_near(initPatch + 16));
			setOpVolume(i, a, pgm_read_byte_near(initPatch + 17));
			setKSR(i, a, pgm_read_byte_near(initPatch + 18));
			setMul(i, a, pgm_read_byte_near(initPatch + 19));
			setDet1(i, a, pgm_read_byte_near(initPatch + 20));
			setDet2(i, a, pgm_read_byte_near(initPatch + 21));
			setAMSenseEn(i, a, pgm_read_byte_near(initPatch + 22));
		}
	}
}



uint8_t YM2151DriverClass::getAdr(uint8_t channel, uint8_t op){
	return (op * 8) + channel;
}



//Default Patch
PROGMEM const unsigned char initPatch[] = {
	0,   0,   0,   0,   0, //LFO
	64,   0,   0 ,  0,   0,  64 ,  0, //CH
	124 ,  0,   0,   32 ,  0 ,  0 ,  0,   8,   0,   0,   0, //OPs
};


YM2151DriverClass YM2151Driver;

