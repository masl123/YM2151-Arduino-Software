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

#ifndef _YM2151DRIVER_h
#define _YM2151DRIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "YM2151.h"


class YM2151DriverClass
{

 private:
	 uint8_t getAdr(uint8_t channel, uint8_t op);

	 //CHANNELS
	 uint8_t ChannelControl[8];
	 uint8_t PhAmp_ModSens[8];
	 uint8_t OpOn[8];

	 //OPS
	 uint8_t TotalLevel[32];
	 uint8_t DetPhase_Mul[32];
	 uint8_t EG_Attack[32];
	 uint8_t EG_Decay1[32];
	 uint8_t EG_Decay2[32];
	 uint8_t EGDec_RelRate[32];

	 //LFO
	 uint8_t Noize;
	 uint8_t LFOFreq;
	 uint8_t PhAmpMod;
	 uint8_t CtrlOut_WF;

	 uint8_t MasterTune = 63;

 public:
	void init();
	void setOpVolume(uint8_t channel, uint8_t op, uint8_t value);
	void setMul(uint8_t channel, uint8_t op, uint8_t value);
	void setDet1(uint8_t channel, uint8_t op, uint8_t value);
	void setDet2(uint8_t channel, uint8_t op, uint8_t value);
	void setKSR(uint8_t channel, uint8_t op, uint8_t value);
	void setATR(uint8_t channel, uint8_t op, uint8_t value);
	void setDec1R(uint8_t channel, uint8_t op, uint8_t value);
	void setDec1L(uint8_t channel, uint8_t op, uint8_t value);
	void setDec2R(uint8_t channel, uint8_t op, uint8_t value);
	void setRel(uint8_t channel, uint8_t op, uint8_t value);
	void setAMSenseEn(uint8_t channel, uint8_t op, uint8_t value);
	void setOpActive(uint8_t channel, uint8_t op, uint8_t value);
	void setAlgorithm(uint8_t channel, uint8_t value);
	void setFeedback(uint8_t channel, uint8_t value);
	void setPMSense(uint8_t channel, uint8_t value);
	void setAMSense(uint8_t channel, uint8_t value);
	void setLFOFreq(uint8_t value);
	void setWaveForm(uint8_t value);

	void setAmpDepth(uint8_t value);
	void setNoiseEnable(uint8_t value);
	void setNoiseFreq(uint8_t value);
	void setCTRLout(uint8_t value);
	void setPhaseDepth(uint8_t value);
	void setPan(uint8_t channel, uint8_t value);
	void noteOn(uint8_t channel);
	void noteOff(uint8_t channel);
	void setTone(uint8_t ch, uint8_t keycode, int16_t kf);
	
	void setMasterTune(uint8_t value);

	void loadInitPatch();
};

extern YM2151DriverClass YM2151Driver;

#endif

