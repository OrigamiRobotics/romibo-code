// romibotest.ino : A simple user program that calls romibo library functions, intended to
//   be used to get familiar with the RomiboRobot library.
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
/****************************************************************/
// This sketch is intended to test minimal functions for Romibo

#include <RomiboSound.h>  
#include <RomiboRobot.h>
#include <ConsoleOutput.h>
#include <mcpDac.h>
#include <Wire.h>


/****************************************************************/

// Declare the default parameter table object.
#include <Parameters.h>
Parameters parameters( NULL, 0 );

// Define a polled serial output stream.
ConsoleOutput Console;

/****************************************************************/
static const char *sounds[] = {
  "HAPPY1",
  "HAPPY2",
  "HAPPY3",
  "ANGRY1",
  "ANGRY2",
  "ANGRY3",
  "SAD1",
  "SAD2",
  "SAD3",
  "SAD4",
  "SAD5",
  "SAD6",
  "BECKON1",
  "BECKON2",
  "BECKON3",
  "BECKON4",
  "BECKON5",
  "BECKON6",
  "AWESOME",
  "GOODBYE",
  "HELLO",
  "LETSPLA1",
  "LETSPLA2",
  "PLAYWME",
  "QUEST1",
  "QUEST2",
  "POWERON",
  "DIGIT0",
  "DIGIT1",
  "DIGIT2",
  "DIGIT3",
  "DIGIT4",
  "DIGIT5",
  "DIGIT6",
  "DIGIT7",
  "DIGIT8",
  "DIGIT9",

};

void setup(void) 
{
  // Set up the com ports
  Serial.begin(57600);      // Set up the terminal com port
  Serial1.begin(9600);      // Set up the WiFi com port

  // Initialize the Romibo system and attach it to the hardware.
  Romibo.begin();

  // Add the low-level variables to the user interface for debugging.
  parameters.addParameters( Romibo.parameters_table(), Romibo.parameters_table_size());

  //setting a beginning head position
  Romibo.setNeutralHeadPosition ();

  //Set up initial antennae color
  Romibo.setAntennaColor (255, 255, 255); // White
  Romibo.delay (1000);
  
}  // end setup

int irRange = 0;
int llight = 0;
int rlight = 0;
int buttons = 0;

int incomingByte = 0;

// Loops indefinitely.
void loop( void )
{     
  // Forces the update the robot inputs and outputs.
  Romibo.poll();
  
  /*
  // IR range goes from ~650 when close, to ~50 when far.
  Serial.println("IR Range");
  Serial.println(analogRead(A1));
  
  // Photoresistor ~1000 is dark. ~200 is flashlight
  Serial.println("Photo Left");
  Serial.println(analogRead(A2));
  Serial.println("Photo Right");
  Serial.println(analogRead(A3));
  */
  
  irRange = Romibo.frontRangeDistance();
  llight = Romibo.leftFrontLightLevel();
  rlight = Romibo.rightFrontLightLevel();
  
  Serial.println(irRange);
  Serial.println(llight);
  Serial.println(rlight);
  
  Romibo.delay(500);
}


