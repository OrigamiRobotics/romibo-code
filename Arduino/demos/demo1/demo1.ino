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

void setup(void) 
{
  // Set up the com ports
  Serial.begin(57600);      // Set up the terminal com port
  Serial1.begin(9600);      // Set up the WiFi com port
  
  // Initialize the Romibo system and attach it to the hardware.
  Romibo.begin();

  // Add the low-level variables to the user interface for debugging.
  parameters.addParameters( Romibo.parameters_table(), Romibo.parameters_table_size());
}

boolean disoriented = false;
// Magnitude of the acceleration vector on accelerometers
int headVecMag = 0;
int bodyVecMag = 0;

// Times for when it was last flipped (upside down, sideways), and last hit.
unsigned long lastFlip;
unsigned long lastHit;


// Loops indefinitely.
void loop( void )
{     
  // Forces the update the robot inputs and outputs. 
  Romibo.poll();
  
  // Access the accelerometer data
  int accXMobo = Romibo.readAccMobo('x');
  int accYMobo = Romibo.readAccMobo('y');
  int accZMobo = Romibo.readAccMobo('z');
  
  int accXHbbo = Romibo.readAccHbbo('x');
  int accYHbbo = Romibo.readAccHbbo('y');
  int accZHbbo = Romibo.readAccHbbo('z');

  // Magnitude of accel vector for head, and body
  headVecMag = sqrt(pow(accXHbbo,2) +
                    pow(accYHbbo,2) +
                    pow(accZHbbo,2));
                    
  bodyVecMag = sqrt(pow(accXMobo,2) +
                    pow(accYMobo,2) +
                    pow(accZMobo,2));
                    
  
  // Calculate the difference in the two accelerometers
  Serial.println("Vec Diff: ");
  Serial.println(headVecMag - bodyVecMag);
  
  /* We set a timer for making the noise when it's hit so it doesn't interrupt
   * the sound being played.
   * We consider a hit when the difference in the vectors is 80, which is
   * (1/3)g. This value can be adjusted to make it more or less sensitive.
   * Note that a magnitude of 255 means a difference of 1g. */
  if(abs(headVecMag - bodyVecMag) > 80 && (millis() > lastHit + 500))
  {
    
     lastHit = millis();
     Serial.println("hit"); 
     Romibo.playSoundNamed("ANGRY2");
     Serial.println("angry");
     
     Serial.print(accXHbbo);
     Serial.print("\t");
     Serial.print(accYHbbo);
     Serial.print("\t");
     Serial.println(accZHbbo);
     Serial.print("Acc Vec Mag: ");
     Serial.println(headVecMag);
  }

  /* We only want to set disoriented to true when it has changed
   * from rightside-up to some disorientation. 
   * The reason behind this is we do not want it to play the 
   * crying sound continuously, but rather at a controllable
   * interval so it doesn't interrupt the previously playing sound
   * Then we begin the timer */
  if(!disoriented && (accYMobo > 200 ||
                      abs(accXMobo) < 200||
                      abs(accZMobo) < 200))
  {
     Serial.println("turned upside down");
     disoriented = true;
     lastFlip = millis(); 
  }
  /* Set disoriented to false when it changes from disoriented to 
   * rightside-up so that it knows when it is not disoriented. */
  else if(disoriented && !(accYMobo >= 200 ||
                        abs(accXMobo) >= 200||
                      abs(accZMobo) >= 200))
  {
     Serial.println("turned rightside up");
     disoriented = false;
  }
  
  /* Reset the timer so that it can set a controlled interval
   * for when the sound will play again if the Romibo remains
   * disoriented */
  if(disoriented && (millis() > lastFlip + 1500))
  {
    Romibo.playSoundNamed("SAD2");
    Serial.println("Sad");
    lastFlip = millis();
  }
}

