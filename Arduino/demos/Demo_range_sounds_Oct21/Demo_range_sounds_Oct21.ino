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
#include <Timer.h>


/****************************************************************/
// Declare the default parameter table object.
#include <Parameters.h>
Parameters parameters( NULL, 0 );

//REFAC: this should be in config file
boolean shouldBlink = false;
boolean shouldWander = false;
boolean shouldDrive = true;  //currently unused, all driving occurs during wandering
boolean shouldRespondToTouch = false;
boolean shouldBob = false;
boolean shouldSway = false;


//blinking
Timer blinkTimer;
Timer bobTimer;
Timer swayTimer;

//acceleration variables
boolean disoriented = false;
int headVecMag = 0;
int bodyVecMag = 0;

// Times for when it was last flipped (upside down, sideways), and last hit.
unsigned long lastFlip;
unsigned long lastHit;

//contact switches
int buttons = 0;
unsigned long lastTouch = 0;

//photo sensors (wander and avoid walls)
int irRange = 0;
int llight = 0;
int rlight = 0;
boolean inWanderingTime = false;
Timer wanderingTimer;

// range audio stuff
int sndIndex = 0;
int sndDelay = 0;
int lastRange = 0;
const int sndCount = 3;
const char sndFiles[sndCount][8] = {"HAPPY1", "HAPPY1", "HAPPY1"};
const int sndDelayConst = 5000;
const int sndRangeConst = 300;

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

  //Set up initial antennae color
  Romibo.setAntennaColor (255, 255, 255); // White
  Romibo.delay (1000);

  // Romibo conveys successful setup with a blink, a nod, a whistle, and a green light
  Romibo.playSoundNamed ("HAPPY1");
  Romibo.setAntennaColorGreen(); // Green 
 
 
  if (shouldBlink)
  {
    blinkRandomly();
  }
  
  if (shouldWander)
  {
    wanderRandomly();
  }
  
  if (shouldBob)
  {
      bobRandomly();
  }

  if (shouldSway)
  {
     swayRandomly(); 
  }

  Romibo.setAntennaColorWhite();

  Romibo.setNeutralHeadPosition();

}

// Loops indefinitely.
void loop( void )
{     
  // Forces the update the robot inputs and outputs. 
  Romibo.poll();

  //update blink timer
  blinkTimer.update();
  
  //check if we're upside-down or if we've been hit
  checkAcceleration();
  
  //check if touch switches have been pressed
  if (shouldRespondToTouch)
  {
    checkTouches();
  }

  // check range for audio playback
  checkRangeForAudio();
  
  //if we're supposed to be wandering, check for light sources and walls
  wanderingTimer.update();
  if (inWanderingTime)
  {
      checkObstructions();
      checkLighting();
  }
  
}

void checkRangeForAudio()
{
  int range = Romibo.frontRangeDistance();

  if (lastRange > sndRangeConst && range <= sndRangeConst && !sndDelay) {
    //play next audio track
    Serial.print(sndFiles[sndIndex]);
    Romibo.playSoundNamed (sndFiles[sndIndex]);
    sndDelay = sndDelayConst;

    sndIndex ++;
    if (sndIndex >= sndCount) {
      sndIndex = 0;
    }
    Serial.print("index "); Serial.print(sndIndex);
  }

  lastRange = range;
  if (sndDelay > 0) sndDelay --;
}


void swayRandomly()
{
   Romibo.sway();
   
   int swayInterval = random(8000,20000);
   Serial.print("Next Sway interval (ms): "); Serial.println(swayInterval);
   
   swayTimer.after(swayInterval, swayRandomly); 
}



void bobRandomly()
{
   Romibo.bob();
   
   int bobInterval = random(8000,20000);
   Serial.print("Next Bob interval (ms): "); Serial.println(bobInterval);
   
   bobTimer.after(bobInterval, bobRandomly); 
}


void wanderRandomly()
{
   inWanderingTime = true;
   
   int wanderInterval = random(8000,20000);
   Serial.print("Stop wandering in (ms): "); Serial.println(wanderInterval);
   
   wanderingTimer.after(wanderInterval, stopWandering);
}

void stopWandering()
{
   inWanderingTime = false;
   Romibo.drive(0,0);
   
   int wanderInterval = random(8000,20000);
   Serial.print("Start wandering in (ms): "); Serial.println(wanderInterval);
   
   wanderingTimer.after(wanderInterval, wanderRandomly);
}

void checkObstructions()
{
   irRange = Romibo.frontRangeDistance();
   
   //Serial.print("IR distance: "); Serial.println(irRange);
   
    while (irRange <= 300)
    {
      Serial.println("Found obstruction");
      
      Romibo.drive (0,100);
      
      Romibo.delay(500);
      Romibo.drive(0,0);
      irRange = Romibo.frontRangeDistance();
    }
  
}

void checkLighting()
{
  llight = Romibo.leftFrontLightLevel();
  rlight = Romibo.rightFrontLightLevel();
  
   int ambient = ((llight + rlight)/2);
   
  //Serial.print ("Left Light: ");
  //Serial.println (llight);
  //Serial.print ("Right Light: ");
  //Serial.println (rlight);
  //Serial.print ("Ambient:");
  //Serial.println (ambient);
  
  if ( abs(llight - ambient) < 10 && abs(rlight- ambient) < 10)
  {
    //Serial.println("Simon says forward");
    Romibo.drive(100, 100);
    return;
  }
   
    if(llight > ambient)
    {
        //Serial.println("Photo resistors say turn left");
        Romibo.drive (0,100);
        
    } 
    else 
    {
        //Serial.println("Photo resistors say turn right");
        Romibo.drive (100,0);
      
    }
}

void checkAcceleration()
{
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
                    
  /* We consider a hit when the difference in the vectors is 80, which is
   * (1/3)g. This value can be adjusted to make it more or less sensitive.
   * Note that a magnitude of 255 means a difference of 1g. */
   // changed thresholds to make it less sensitive (JM)
  if(abs(headVecMag - bodyVecMag) > 400 && (millis() > lastHit + 500))
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
     //Serial.println("turned upside down (disoriented true)");
     disoriented = true;
     lastFlip = millis(); 
  }
  /* Set disoriented to false when it changes from disoriented to 
   * rightside-up so that it knows when it is not disoriented. */
  else if(disoriented && !(accYMobo >= 200 ||
                        abs(accXMobo) >= 200||
                      abs(accZMobo) >= 200))
  {
     //Serial.println("turned rightside up (disoriented false)");
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

void checkTouches()
{ 
  buttons = Romibo.touchSwitches();
  
  if (buttons != 0) {
      beenTouched();
  }

}

void beenTouched()
{
  //don't respond to petting more than once every 1.5 seconds
  if (millis() - lastTouch <  1500)
    return;
  else lastTouch = millis();

  if (shouldBlink)
  {
    Romibo.blink();
  }
  
  int r = random(0, 100);
   
  //half the time, whistle when petted 
  if (r < 51)
  {
    Romibo.playSoundNamed("S1"); 
  }
    
}

void blinkRandomly()
{
   Romibo.blink();
   
   int blinkInterval = random(8000,20000);
   Serial.print("Next Blink interval (ms): "); Serial.println(blinkInterval);
   
   blinkTimer.after(blinkInterval, blinkRandomly);
}


