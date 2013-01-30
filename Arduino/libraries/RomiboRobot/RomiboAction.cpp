// RomiboActions.cpp: A simple control library for the Romibo robot,
// which focuses on simple behaviors.
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
#include <RomiboAction.h>
  
#define ROA_HEAD_UP 100
#define ROA_HEAD_DOWN 0
#define ROA_BOB_DELAY 300
  
#define ROA_NOD_DELAY 400
  
#define ROA_EYES_OPEN 100
#define ROA_EYES_CLOSED 0
#define ROA_BLINK_DELAY 100
  
#define ROA_MAINTAINDIST_SPEED 65
#define ROA_MAINTAINDIST_MAXTIME 2000
#define ROA_APPROACH_SPEED 60
#define ROA_APPROACH_MAXTIME 2000
  
#if ROMIBO_ELECTRONICS_MAJOR_REV==2
#define ROA_DRIVE_SPEED 100
#endif

#ifndef ROA_DRIVE_SPEED
#define ROA_DRIVE_SPEED 70
#endif

void RomiboAction::bob() {
  Romibo.setHeadPosition (ROA_HEAD_UP, ROA_HEAD_UP);
  Romibo.delay (ROA_BOB_DELAY); // 100/1000 sec delay
  Romibo.setHeadPosition (ROA_HEAD_DOWN, ROA_HEAD_DOWN);
  Romibo.delay (ROA_BOB_DELAY);
  Romibo.setHeadPosition (ROA_HEAD_UP, ROA_HEAD_UP);
  Romibo.delay (ROA_BOB_DELAY);
  Romibo.setNeutralHeadPosition ();
}

void RomiboAction::bobs (int numbobs) {
  Romibo.setHeadPosition (ROA_HEAD_UP, ROA_HEAD_UP);
  Romibo.delay (ROA_BOB_DELAY);
  for (int i = 0; i < numbobs; i++) {
    Romibo.setHeadPosition (ROA_HEAD_DOWN, ROA_HEAD_DOWN);
    Romibo.delay (ROA_BOB_DELAY);
    Romibo.setHeadPosition (ROA_HEAD_UP, ROA_HEAD_UP);
    Romibo.delay (ROA_BOB_DELAY);
  }
  Romibo.setNeutralHeadPosition ();

}

void RomiboAction::nod() {
  Romibo.setHeadPosition (ROA_HEAD_DOWN, ROA_HEAD_UP);
  Romibo.delay (ROA_NOD_DELAY);
  Romibo.setHeadPosition (ROA_HEAD_UP, ROA_HEAD_DOWN);
  Romibo.delay (ROA_BOB_DELAY);
  Romibo.setNeutralHeadPosition ();
}
  
void RomiboAction::nods (int numnods) {
  Romibo.setHeadPosition (ROA_HEAD_DOWN, ROA_HEAD_UP);
  Romibo.delay (ROA_NOD_DELAY);
	for (int i = 0; i < numnods; i++) {
	  Romibo.setHeadPosition (ROA_HEAD_UP, ROA_HEAD_DOWN);
	  Romibo.delay (ROA_BOB_DELAY);
	  Romibo.setHeadPosition (ROA_HEAD_DOWN, ROA_HEAD_UP);
	  Romibo.delay (ROA_BOB_DELAY);
	}
  Romibo.setNeutralHeadPosition ();
}

void RomiboAction::blink() {
  Romibo.setEyelid (ROA_EYES_CLOSED);
  Romibo.delay (ROA_BLINK_DELAY);
  Romibo.setEyelid (ROA_EYES_OPEN);
}

void RomiboAction::blinks(int numblinks) {
  for (int i = 0; i < numblinks; i++) {
	Romibo.setEyelid (ROA_EYES_CLOSED);
	Romibo.delay (ROA_BLINK_DELAY);
	Romibo.setEyelid (ROA_EYES_OPEN);
	Romibo.delay (ROA_BLINK_DELAY);
  }
}

void RomiboAction::maintainDistance(int dist) {  
  int time = 0;
  while (Romibo.frontRangeDistance () < dist && time < ROA_MAINTAINDIST_MAXTIME) {
	Romibo.delay (10);  
	time += 10;
	Romibo.driveBackward (ROA_MAINTAINDIST_SPEED);
  }
  Romibo.stop();
  
}

void RomiboAction::approach (int dist) {
  int time = 0;
  while (Romibo.frontRangeDistance () > dist && time < ROA_APPROACH_MAXTIME) {
	Romibo.delay (10);
	time += 10;
	Romibo.driveForward (ROA_APPROACH_SPEED);
  }
  Romibo.stop();
}

void RomiboAction::driveForwardTimed(int time) {
  int t = 0;
  Romibo.driveForward (ROA_DRIVE_SPEED);
  while (t < time) {
	Romibo.delay (10);
	t += 10;
  }
  Romibo.stop ();
}
  
void RomiboAction::driveBackwardTimed(int time) {
  int t = 0;
  Romibo.driveBackward (ROA_DRIVE_SPEED);
  while (t < time) {
	Romibo.delay (10);
	t += 10;
  }
  Romibo.stop ();
}
  
void RomiboAction::turnRight (int time) {
  int t = 0;
  Romibo.drive (ROA_DRIVE_SPEED, -1 * ROA_DRIVE_SPEED);
  while (t < time) {
	Romibo.delay (10);
	t += 10;
  }
  Romibo.stop ();

}
  
void RomiboAction::turnLeft (int time) {
  int t = 0;
  Romibo.drive ( -1 * ROA_DRIVE_SPEED, ROA_DRIVE_SPEED);
  while (t < time) {
	Romibo.delay (10);
	t += 10;
  }
  Romibo.stop ();
}
  
void RomiboAction::turnTowardLight () {
  char initlightdir;
  if (Romibo.leftFrontLightLevel () > Romibo.rightFrontLightLevel ()) {
	initlightdir = 'l';
	Romibo.drive (-1 * ROA_DRIVE_SPEED, ROA_DRIVE_SPEED);
  } else if (Romibo.rightFrontLightLevel () > Romibo.leftFrontLightLevel ()) {
	initlightdir = 'r';
	Romibo.drive (ROA_DRIVE_SPEED, -1 * ROA_DRIVE_SPEED);
  } else return;
  char lightdir = initlightdir;
  while (1) {
	if (Romibo.leftFrontLightLevel () > Romibo.rightFrontLightLevel ()) {
	  lightdir = 'l';
	} else if (Romibo.rightFrontLightLevel () > Romibo.leftFrontLightLevel ()) {
	  lightdir = 'r';
	} else lightdir = 0;
  
    if (lightdir != initlightdir) return;
  }
  
}
  
void RomiboAction::driveTowardLight(int time) {
  int t = 0;
  int lightDifference = 0;
  while (t < time) {
    Romibo.delay (10);
    t+=10;
	lightDifference = Romibo.leftFrontLightLevel () - Romibo.rightFrontLightLevel();
	Romibo.drive (ROA_DRIVE_SPEED - lightDifference, ROA_DRIVE_SPEED + lightDifference);   
  }
}
  
void RomiboAction::driveAwayFromLight(int time) {
  int t = 0;
  int lightDifference = 0;
  while (t < time) {
    Romibo.delay (10);
    t++;
	lightDifference = Romibo.leftFrontLightLevel () - Romibo.rightFrontLightLevel();
	Romibo.drive (ROA_DRIVE_SPEED + lightDifference, ROA_DRIVE_SPEED - lightDifference);   
  }
}

void RomiboAction::setAntenna (romiboaction_color_t color) {
  switch (color) {
	case (ROA_RED):
	  Romibo.setAntennaColor (100, 0, 0);
	  break;
    case (ROA_YELLOW):
	  Romibo.setAntennaColor (100, 100, 0);
	  break;
	case (ROA_GREEN):
	  Romibo.setAntennaColor (0, 100, 0);
	  break;
	case (ROA_AQUA):
	  Romibo.setAntennaColor (0, 100, 100);
	  break;
	case (ROA_BLUE):
	  Romibo.setAntennaColor (0, 0, 100);
	  break;
	case (ROA_VIOLET):
	  Romibo.setAntennaColor (100, 0, 100);
	  break;
    default:
	  break;
  }
  
}
  
/*
  //TODO: implement
void RomiboAction::waitForButton (int max_time, int button) {
  int buttonnum = 0;
  switch (button) {
	case (ROA_TOP):
  }
  return;
}
*/
  
void RomiboAction::waitForTargetRange (int max_time, int min_range, int max_range) {
  int t = 0;
  int range = Romibo.frontRangeDistance ();
  while (t < max_time && (range < min_range || range > max_range)) {
	t+=10;
	range = Romibo.frontRangeDistance ();
	Romibo.delay (10);
  }
}
  
/* instanciates RomiboAct, thus any arduino sketch that has
 * #include RomiboAction.h can call, for instance, RomiboAct.bob() */
RomiboAction RomiboAct;