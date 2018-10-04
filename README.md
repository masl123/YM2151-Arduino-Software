# YM2151-Arduino-Software
Software for the ym2151-Shield for Arduino


This is a Firmware to control the YM2151 Shield shown at this Website: http://www.ooishoo.org/
<br>You also can look at this Thread: http://chipmusic.org/forums/topic/17710/ym2151-shield-for-arduino/ for more informations about the Shield. 
<br><br>
This Firmware allows you to control the YM2151-Synthesizer Chip over Midi. This includes parameters to control the timbre of the instrument, but also to choose from either the polyphonic-mode, where notes are assigned automatically to the eight voices of the Chip or the monophonic-mode, where for each voice-channel the timbre and the notes can be controlled.


#### Polyphonic-mode:
The notes which are sent over midi are assigned automatically to one of the free voices of the chip. All voices have the same timbre. There can be a maximum of eight notes playing at the same time. 
Midi notes and control change messages have to be sent over midi channel 1. The control messages which can be sent in polyphonic-mode over midi channel one, are described in [Mapping.md](Mapping.md). 


#### Monophonic-mode:
This mode allows you to control up to eight different monophonic voices with different timbres.
The notes and control change messages which are sent over midi are assigned to the same Voice as the Midi Channel number. So sending a message over midi channel 5, only affects voice no. 5 of the YM2151 Chip. As the YM2151-Chip has eight voices, only midi channels one through eight are used. The control messages which can be sent in monophonic-mode over midi channels one through eight, are described in [Mapping.md](Mapping.md).

<br>- See [Mapping.md](Mapping.md) for the control-messages you can send over midi to control the parameters of the chip.
<br>- See [LICENSE.md](LICENSE.md) for the Licensing terms.

<br>

### Requirements:
The Arduino Midi Library:
	https://github.com/FortySevenEffects/arduino_midi_library

<br>

### Controller Software
To make the use of the firmware a bit easier, there is a Controller Software for PC and Mac available which allows you to control the functions of the firmware:
https://github.com/masl123/YM2151-Midi-Controller
