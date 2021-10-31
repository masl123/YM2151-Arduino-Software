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

#include  <EEPROM.h>

#include "EPROMManager.h"


void EPROMManagerClass::init()
{
}

uint8_t* EPROMManagerClass::load(uint16_t address, uint8_t length)
{
	uint8_t* values = new uint8_t[length];
	for (uint8_t i = 0; i < length; i++) {
		values[i] = EEPROM.read(address + i);
	}
	return values;
}

void EPROMManagerClass::save(uint16_t address, uint8_t values[], uint8_t length)
{
	for (uint8_t i = 0; i < length; i++) {
		EEPROM.write(address + i, values[i]);
	}
}

EPROMManagerClass EPROMManager;

