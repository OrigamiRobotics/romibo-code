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
// Magnitude of the acceleration vector on accelerometer for the breakout board
int headVecMag = 0;
int bodyVecMag = 0;
unsigned long lastFlip;
unsigned long lastHit;


// Loops indefinitely.
void loop( void )
{     
  // Forces the update the robot inputs and outputs. 
  Romibo.poll();
    
  int accXMobo = Romibo.readAccMobo('x');
  int accYMobo = Romibo.readAccMobo('y');
  int accZMobo = Romibo.readAccMobo('z');
  
  int accXHbbo = Romibo.readAccHbbo('x');
  int accYHbbo = Romibo.readAccHbbo('y');
  int accZHbbo = Romibo.readAccHbbo('z');

     Serial.print("Mobo: ");
     Serial.print(accXMobo);
     Serial.print("\t");
     Serial.print(accYMobo);
     Serial.print("\t");
     Serial.println(accZMobo);

     Serial.print("Hbbo: ");
     Serial.print(accXHbbo);
     Serial.print("\t");
     Serial.print(accYHbbo);
     Serial.print("\t");
     Serial.println(accZHbbo);

  // Magnitude of accel vector for head
  headVecMag = sqrt(pow(accXHbbo,2) +
                    pow(accYHbbo,2) +
                    pow(accZHbbo,2));
                    
  bodyVecMag = sqrt(pow(accXMobo,2) +
                    pow(accYMobo,2) +
                    pow(accZMobo,2));
                    
                 
  Serial.println("Vec Diff: ");
  Serial.println(headVecMag - bodyVecMag);
  
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

  if(!disoriented && (accYMobo > 200 ||
                      accXMobo > 200 ||
                      accXMobo < -200 ||
                      accZMobo > 200 ||
                      accZMobo < -200))
  {
     Serial.println("turned upside down");
     disoriented = true;
     lastFlip = millis(); 
  }
  else if(disoriented && !(accYMobo > 200 ||
                        accXMobo > 200 ||
                        accXMobo < -200 ||
                        accZMobo > 200 ||
                        accZMobo < -200))
  {
     Serial.println("turned rightside up");
     disoriented = false;
  }
  
  if(disoriented && (millis() > lastFlip + 1000))
  {
    Romibo.playSoundNamed("SAD2");
    Serial.println("Sad");
    lastFlip = millis();
  }
  
  delay(500);
}

