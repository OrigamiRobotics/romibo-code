// RomiboExample3.ino : button callbacks
// Copyright (c) 2012 John Frens
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
// This sketch is intended to test the minimal requirements for using the Romibo
// libraries.

#include <RomiboSound.h>  
#include <RomiboRobot.h>
#include <RomiboAction.h>
#include "ConsoleOutput.h"
/****************************************************************/
// Declare the default parameter table object.
#include <Parameters.h>
Parameters parameters( NULL, 0 );

// Define a polled serial output stream.
ConsoleOutput Console;  

/****************************************************************/
void bob_whistle (unsigned x) {
  x = x; // to get rid of any warnings for unused variables
  RomiboAct.bob();
  Romibo.playSoundNamed ("HAPPY1");
}

void setup(void) 
{
  Serial.begin(57600);
  Serial1.begin(9600);

  // Initialize the Romibo system and attach it to the hardware.
  Romibo.begin();

  // Add the low-level variables to the user interface for debugging.
  parameters.addParameters( Romibo.parameters_table(), Romibo.parameters_table_size());

  Serial.println("INITIALIZED");
  
  Romibo.set_button_callback(&bob_whistle);
}

void loop( void )
{
  // Update the robot inputs and outputs.
  Romibo.poll();
}
