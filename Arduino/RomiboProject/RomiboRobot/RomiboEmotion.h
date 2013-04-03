// RomiboEmotions.h : An emotion library
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

// This library is intended to provide convienient emotions for Romibo
// which can run in the background, as well as emotional reactions
// such as a happy bob or a sad nod.

// This is not an exhaustive library of emotions, but is intended to
// provide convienient and relatively universal behaviors to the
// user, both for sequential use and background execution.

// RomiboEmotions.h is not included by any of the other libraries and therefore
// must be included by the user if applicable.

#ifndef __ROMIBOEMOTION_H_DEFINED__
#define __ROMIBOEMOTION_H_DEFINED__

class RomiboEmotion {

public:

  // Background Emotions
  void actSad ();
  void actNeutral ();
  void actElated ();
  void actUnsatisfied ();
  void actIrate ();
  void actSatisfied ();
  void stopBackgroundEmotions ();

  // Other Background Behavior
  void startBobbing ();
  void stopBobbing ();
  void startRocking ();
  void stopRocking ();
  // freq indicates average number of times per minute
  void blinkLights (int freq, int color); //TODO: define enum for color
  void pulseLights (int freq, int color);
  void stopLights ();
  void blinkEyesRandomly (int averagefreq);
  void stopBlinkingEyes ();

  // Foreground Emotional Reactions
  void happyReaction ();
  void sadReaction ();
  void neutralReaction ();
  void elatedReaction ();
  void unsatisfiedReaction ();
  void irateReaction ();
  void satisfiedReaction ();
};

extern RomiboEmotion RomiboEmote;

#endif __ROMIBOEMOTION_H_DEFINED__