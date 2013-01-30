// RomiboSound.h : wrapper code for the WaveHC library, customized for the Romibo robot
// Copyright (c) 2011 Aubrey Shick, Brian Pfiffner, Garth Zeglin
// ##BEGIN_LICENSE##
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this file; if not, write to the Free Software Foundation,
// Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
// ##END_LICENSE##
/****************************************************************/

#ifndef __ROMIBOSOUND_H_INCLUDED__
#define __ROMIBOSOUND_H_INCLUDED__

#include "Arduino.h"

extern int sdSetup(void);
extern void sdErrorCheck(void);
extern void playfile(char* name);
extern void stopSound(void);

extern boolean soundEnabled(void);
extern boolean soundIsPlaying(void);

#endif // __ROMIBOSOUND_H_INCLUDED__

