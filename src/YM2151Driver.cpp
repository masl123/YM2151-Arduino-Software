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



//TODO Use Most Significant Bits instead of LSBs for Values (Top part of byte instead of bottom part) e.g.
//
//	((value & 0xf) << 4       -->   ((value & 0x78) << 1)
//	((value & 0x3) << 6)	  -->	((value & 0x60) << 1)
//	(value & 0x0F)			  -->	((value & 0x78) >> 3)
//
// ... DONE, but needs Testing

#include "YM2151Driver.h"

void YM2151DriverClass::init()
{
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


void	YM2151DriverClass::setVolume(uint8_t ch, uint8_t volume, uint16_t offset)
{
	static  uint8_t	volume_tbl[] = {
		0x2a, 0x28, 0x25, 0x22, 0x20, 0x1d, 0x1a, 0x18,
		0x15, 0x12, 0x10, 0x0d, 0x0a, 0x08, 0x05, 0x02,
	};
	int16_t	tl, att;
	if (volume & (0x80)){
		tl = volume & 0x7f;
	}
	else {
		if (volume > 15){
			volume = 15;
		}
		tl = volume_tbl[volume];
	}
	tl += offset >> 8;
	for (int i = 0; i<4; i++){
		if (OpOn[ch] & (1 << i)){
			att = TotalLevel[getAdr(ch,i)] + tl;
		}
		else {
			att = TotalLevel[getAdr(ch, i)];
		}
		if (att > 0x7f || att < 0) att = 0x7f;
		YM2151.write(0x60 + i * 8 + ch, att);
	}
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
void YM2151DriverClass::setTone(uint8_t ch, uint8_t keycode, int16_t kf){
	int16_t	offset_kf = (kf & 0x3f);
	int16_t	offset_note = keycode + (kf >> 6);
	if (offset_note < 0) offset_note = 0;
	if (offset_note > 0xbf) offset_note = 0xbf;

	YM2151.write(0x30 + ch, offset_kf << 2);
	YM2151.write(0x28 + ch, pgm_read_byte_near(KeyCodeTable + offset_note));
}


void	YM2151DriverClass::loadTimbre(uint8_t ch, uint16_t prog_addr)
{
	static uint8_t carrier_slot_tbl[] = {
		0x08, 0x08, 0x08, 0x08, 0x0c, 0x0e, 0x0e, 0x0f,
	};

	uint16_t taddr = prog_addr;
	uint8_t	no = pgm_read_byte_near(taddr++);
	
	ChannelControl[ch] = pgm_read_byte_near(taddr++);
	YM2151.write(0x20 + ch, ChannelControl[ch]);

	OpOn[ch] = pgm_read_byte_near(taddr++);

	for (int i = 0; i<32; i += 8){
		uint8_t	dt1_mul = pgm_read_byte_near(taddr++);
		DetPhase_Mul[i + ch] = dt1_mul;
		YM2151.write(0x40 + ch + i, dt1_mul);
	}

	for (int i = 0; i<4; i++){
		TotalLevel[getAdr(ch,i)] = pgm_read_byte_near(taddr++);
	}

	for (int i = 0; i<32; i += 8){
		uint8_t	ks_ar = pgm_read_byte_near(taddr++);
		EG_Attack[i + ch] = ks_ar;
		YM2151.write(0x80 + ch + i, ks_ar);
	}

	for (int i = 0; i<32; i += 8){
		uint8_t	ame_d1r = pgm_read_byte_near(taddr++);
		EG_Decay1[i + ch] = ame_d1r;
		YM2151.write(0xa0 + ch + i, ame_d1r);
	}

	for (int i = 0; i<32; i += 8){
		uint8_t	dt2_d2r = pgm_read_byte_near(taddr++);
		EG_Decay2[i + ch] = dt2_d2r;
		YM2151.write(0xc0 + ch + i, dt2_d2r);
	}

	for (int i = 0; i<32; i += 8){
		uint8_t	d1l_rr = pgm_read_byte_near(taddr++);
		EGDec_RelRate[i + ch] = d1l_rr;
		YM2151.write(0xe0 + ch + i, d1l_rr);
	}

}


uint8_t YM2151DriverClass::getAdr(uint8_t channel, uint8_t op){
	return (op * 8) + channel;
}

YM2151DriverClass YM2151Driver;

