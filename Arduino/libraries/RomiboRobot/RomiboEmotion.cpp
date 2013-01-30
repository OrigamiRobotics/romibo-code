// RomiboEmotions.cpp : An emotion library
// 
// Copyright (c) 2011 John Frens

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

#include <RomiboRobot.h>
#include <RomiboEmotion.h>
#include <RomiboAction.h>
#include <Timeline.h>
#include <stdlib.h>

Timeline *emotionTimeline = 0;

// defaultpos is an int casted as a void *
void blinkfunc (void *defaultpos) {
  int up = (int)defaultpos;
  int down = 0;

  if (rand() % 2 == 1) {
    Romibo.setEyelid (up);
  }
  if (rand() % 10 == 4) {
    Romibo.setEyelid (down);
  }
}

void RomiboEmotion::actSad () {
  if (emotionTimeline) {
    //stopBackgroundEmotions();
  }
  emotionTimeline = new Timeline;
  emotionTimeline->addEntry(200, &blinkfunc, (void *)60);
  
  RomiboAct.setAntenna(ROA_BLUE);
  
  //SadReaction();
  emotionTimeline->loopInBackground();
}
/*
void RomiboEmotion::actNeutral ();
void RomiboEmotion::actElated ();
void RomiboEmotion::actUnsatisfied ();
void RomiboEmotion::actIrate ();
void RomiboEmotion::actSatisfied ();
void RomiboEmotion::stopBackgroundEmotions ();

// Other Background Behavior
void RomiboEmotion::startBobbing ();
void RomiboEmotion::stopBobbing ();
void RomiboEmotion::startRocking ();
void RomiboEmotion::stopRocking ();
void RomiboEmotion::blinkLights (int freq, int color); //TODO: define enum for color
void RomiboEmotion::pulseLights (int freq, int color);
void RomiboEmotion::stopLights ();
void RomiboEmotion::blinkEyesRandomly (int averageFreq);
void RomiboEmotion::stopBlinkingEyes ();

// Foreground Emotional Reactions
void RomiboEmotion::happyReaction () {
//  Romibo.
}
void RomiboEmotion::sadReaction ();
void RomiboEmotion::neutralReaction ();
void RomiboEmotion::elatedReaction ();
void RomiboEmotion::unsatisfiedReaction ();
void RomiboEmotion::irateReaction ();
void RomiboEmotion::satisfiedReaction ();
*/
RomiboEmotion RomiboEmote;
