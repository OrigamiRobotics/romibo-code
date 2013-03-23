// RomiboConfig.h : source code configuration for the Romibo robot.
// Copyright (c) 2011 Garth Zeglin

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
// This file defines user-configurable symbols which control various
// compile-time features of the Romibo software system.

#ifndef __ROMIBOCONFIG_H_INCLUDED__ 
#define __ROMIBOCONFIG_H_INCLUDED__ 

/****************************************************************/
// User-local configuration.  The following defines allow the user to configure
// the library for a particular installation.

#define ROMIBO_ELECTRONICS_MAJOR_REV 3
#define ROMIBO_SERIAL_NUMBER 1

// Turn on to enable the class-D audio amplifier output.
//#define ROMIBO_AUDIO_PWM_ENABLED 1

// Temporary fix to limit driver heating.
// #define ROMIBO_DRIVE_MOTOR_LIMIT 75

/****************************************************************/
// Define the version of the electronics installed on the target robot.
//  Values for the major version number:
//   0   pre-Shield hand-wired version
//   1   Romibo Shield Rev 1
//   2   Romibo Shield Rev 2  (e.g. includes microphone)
//   3   Romibo Shield Rev 3  (e.g. includes new motor drivers, accelerometer, etc.)

#ifndef ROMIBO_ELECTRONICS_MAJOR_REV 
#define ROMIBO_ELECTRONICS_MAJOR_REV 3
#endif

#ifndef ROMIBO_ELECTRONICS_MINOR_REV
#define ROMIBO_ELECTRONICS_MINOR_REV 0
#endif

// Define which instance of the robot to assume for calibration and
// configuration.  Ideally the robot would automatically determine its identity,
// perhaps by reading a file from the SD card.
#ifndef ROMIBO_SERIAL_NUMBER
#define ROMIBO_SERIAL_NUMBER 0
#endif

// Configure whatever possible from the serial number.
#if ROMIBO_SERIAL_NUMBER==1
#define ROMIBO_WIFLY_INSTALLED 1
#endif

// Set this true if the WiFly WiFi interface is wired to Serial1.
#ifndef ROMIBO_WIFLY_INSTALLED
#define ROMIBO_WIFLY_INSTALLED 0
#endif

// Global motor drive value limit.  (This can probably go away later.)
#ifndef ROMIBO_DRIVE_MOTOR_LIMIT
#define ROMIBO_DRIVE_MOTOR_LIMIT 100
#endif

// Set this true if the audio system generate a PWM audio signal using OC1C.
#ifndef ROMIBO_AUDIO_PWM_ENABLED
#define ROMIBO_AUDIO_PWM_ENABLED 0
#endif

#endif // __ROMIBOCONFIG_H_INCLUDED__ 

