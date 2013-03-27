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
#include <I2C.h>
#include <MMA8453_n0m1.h>

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

void playallsounds()
{
  int len = 37;
  Romibo.waitForSoundToFinish ();
  for (int i = 0; i < len; i++) {
    Romibo.playSoundNamed (sounds[i]);
    Romibo.waitForSoundToFinish ();
    if (i % 3 == 2) {
      delay (2000);

    }
  }
}

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

  // Romibo conveys successful setup with a blink, a nod, a whistle, and a green light
  int head_up = 95;
  int head_down = 5;
  Romibo.playSoundNamed ("HAPPY1");
  Romibo.setAntennaColor (0, 255, 0); // Green
  Romibo.setHeadPosition (head_down, head_up);
  int lid_down = 10;
  int lid_up = 100;
  Romibo.setEyelid (lid_down);
  Romibo.delay (2000);
  Romibo.setHeadPosition (head_up, head_down);
  Romibo.setEyelid (lid_up);
  Romibo.setAntennaColor (255, 255, 255); // White

  //Romibo runs each of its drive motors
  /*
  Romibo.drive (100, 0);
  Romibo.delay (500);
  Romibo.drive (0, 100);
  Romibo.delay (500);
  Romibo.drive (-100, 0);
  Romibo.delay (500);
  Romibo.drive (0, -100);
  Romibo.delay (500);
  Romibo.stop ();
*/
  Romibo.setNeutralHeadPosition ();   

  //playallsounds();

}  // end setup

int irRange = 0;
int llight = 0;
int rlight = 0;
int buttons = 0;

int incomingByte = 0;

// Loops indefinitely.
void loop( void )
{ 
  
  // Reading inputs coming over Serial
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.println ("Serial1:");
    Serial.println (incomingByte,DEC);
  }
  
  // Reading inputs coming over wifi
  if (Serial1.available() > 0) {
    incomingByte = Serial1.read();
    Serial1.println ("Serial1:");
    Serial1.println (incomingByte,DEC);
  }
  
    
  // Forces the update the robot inputs and outputs.
  Romibo.poll();
  int accX = Romibo.readAccX();
  int accY = Romibo.readAccY();
  int accZ = Romibo.readAccZ();
  Serial.print("X: ");
  Serial.print(accX);
  Serial.print("\t Y: ");
  Serial.print(accY);
  Serial.print("\t Z: ");
  Serial.print(accZ);
  Serial.println();

  
  /*
  irRange = Romibo.frontRangeDistance();
  llight = Romibo.leftFrontLightLevel();
  rlight = Romibo.rightFrontLightLevel();
  buttons = Romibo.touchSwitches();
  if (buttons != 0) {
    Romibo.setEyelid(10);
    Romibo.delay(200);
    Romibo.setEyelid(100); 
  }

  Romibo.setAntennaColor (0, 0, irRange); 
  Serial.print ("Range: ");
  Serial.println (irRange);
  Serial.print ("Left Light: ");
  Serial.println (llight);
  Serial.print ("Right Light: ");
  Serial.println (rlight);
  Serial.print ("Buttons: ");
  Serial.println (buttons);
  Serial.println ();
*/
  Romibo.drive (100, 0);
  Romibo.delay (500);
  Romibo.stop();
  Romibo.delay(250);

}


