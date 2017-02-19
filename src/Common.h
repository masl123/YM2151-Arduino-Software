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

#if !defined( YM_COMMON_H_INCLUDED )
#define YM_COMMON_H_INCLUDED
#include	"Arduino.h"

#define		_DEBUG

#ifdef	_DEBUG
#define		ASSERT(msg)
#define		PRINTH(msg,data)
#else
#define		ASSERT(msg)
#define		PRINTH(msg,data)
#endif

#endif  //YM_COMMON_H_INCLUDED
