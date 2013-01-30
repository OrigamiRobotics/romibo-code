// RomiboActions.h : A simple control library for the Romibo robot,
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

// This library is intended to provide convienient, simple behavior
// calls. All functions are implemented in the abstract, relying on
// calls to RomiboRobot.cpp rather than any direct calls to hardware methods.

// This library is intended to be sequential, simple, and user friendly. Note
// that some of the functions may be redundant with RomiboRobot.cpp

// RomiboActions.h is not included by any of the other libraries and therefore
// must be included by the user if applicable.

#ifndef __ROMIBOACTION_H_INCLUDED__
#define __ROMIBOACTION_H_INCLUDED__

enum romiboaction_color_t { ROA_RED, ROA_YELLOW,
	ROA_GREEN, ROA_AQUA, ROA_BLUE, ROA_VIOLET };
  
enum romiboaction_button_t {ROA_TOP, ROA_RIGHT,
	ROA_LEFT, ROA_BOTTOM, ROA_ANY};

class RomiboAction
{

public:

  /* Causes Romibo to move its head vertically down and back up */
  void bob();

  /* Same as bob(), but repeated numbobs times */
  void bobs (int numbobs);  

  /* Causes Romibo to move the front of his head vertically down and back up */
  void nod();

  /* same as nod(), but repeated numnods times */  
  void nods (int numnods);

  /* Causes Romibo to close and open its eyes */
  void blink();

  /* Same as blink(), but repeated numblinks times */
  void blinks(int numblinks);

  /* Causes Romibo to move backward until Romibo.frontRangeDistance() >= dist
   * or ROA_MAINTIAINDIST_MAXTIME ms have passed  */
  void maintainDistance(int dist);

  /* Causes Romibo to move forward until Romibo.frontRangeDistance() <= dist
   * or ROA_APPROACH_MAXTIME ms have passed  */
  void approach (int dist);

  /* Causes Romibo to move forward for time ms (1/1000 sec) */
  void driveForwardTimed(int time);

  /* Causes Romibo to move backward for time ms (1/1000 sec) */
  void driveBackwardTimed(int time);
  
  /* Causes Romibo to rotate clockwise for time ms */
  void turnRight (int time);
  
  /* Causes Romibo to rotate counter-clockwise for time ms */
  void turnLeft (int time);
  
  /* Causes Romibo to stay in place and turn in the direction of light */
  void turnTowardLight ();
  
  /* Causes Romibo to track light while driving forward, for time ms */
  void driveTowardLight(int time);
  
  /* Cause Romibo to track the direction of least light while
   * driving forward, for time ms */
  void driveAwayFromLight(int time);

  /* Sets the color of Romibo's antennae to the value of
   * color (a romiboaction_color_t enum defined above) */
  void setAntenna (romiboaction_color_t color);
  
  /* causes romibo to sleep until the correct button is pressed */
  //void waitForButton (int max_time, romiboaction_button_t button);
  // unimplemented
  
  /* causes Romibo to delay until Romibo.frontRangeDistance() returns a
   * value between min_range and max_range (both inclusive)
   * or until max_time ms have expired   */
  void waitForTargetRange (int max_time, int min_range, int max_range);
  
};
  
//Global object for accessing the RomiboAction library
extern RomiboAction RomiboAct;

#endif //__ROMIBOACTION_H_INCLUDED__
