// RomiboExample2.ino : A simple Romibo Program
// Copyright (c) 2012 Garth Zeglin
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
// This sketch is intended to demonstrate the polling loop callback feature.
// We will set the callback to a function that
// makes Romibo's lights pulse. as a result, Romibo's lights will pulse as
// a background function while a simple sequantial function executes.

#include <RomiboSound.h>  
#include <RomiboRobot.h>
#include "ConsoleOutput.h"
/****************************************************************/
// Declare the default parameter table object.
#include <Parameters.h>
Parameters parameters( NULL, 0 );

// Define a polled serial output stream.
ConsoleOutput Console;  

/****************************************************************/
int count = 0;

// Increments count and sets the antenna to green at increasing
// intensity, resetting once 256 is reached.
void pulse_green () {
  Romibo.setAntennaColor (0, count % 256, 0);
  count++;
  
}

void pulse_red () {
  Romibo.setAntennaColor (count % 256, 0, 0);
  count++;
  
}


// whistles and blinks
void whistle_blink () {
  Romibo.waitForSoundToFinish ();
  Romibo.playSoundNamed ("HAPPY2");
  Romibo.delay (1000);
  Romibo.setEyelid (0);
  Romibo.delay (200);
  Romibo.setEyelid (100);
  Romibo.delay (1000);
}

//executes once
void setup(void) 
{
  // Typical setup calls
  Serial.begin(57600);
  Serial1.begin(9600);
  Romibo.begin();
  parameters.addParameters( Romibo.parameters_table(), Romibo.parameters_table_size());
  Serial.println("INITIALIZED");
  
  // Sets the polling callback to make the antennae pulse green 
  // as part of the background polling cycle.
  int greenid = Romibo.add_polling_callback (&pulse_green);
  int redid;
  
}

//executes in an infinite loop after setup ()
void loop( void )
{
  Romibo.delay (5000);
  whistle_blink ();
  int success = Romibo.remove_polling_callback (greenid);
  if (success != 0) Romibo.playSoundNamed("ANGRY1");
  redid = Romibo.add_polling_callback (&pulse_red);
  Romibo.delay (5000);
  whistle_blink ();
  int success = Romibo.remove_polling_callback (redid);
  if (success != 0) Romibo.playSoundNamed("ANGRY1");
  redid Romibo.add_polling_callback (&pulse_green);
  
  
}
