// ElapsedTimer.cpp : measure arbitrary time intervals using the microseconds clock.
// Copyright (c) 2011 Garth Zeglin

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

#include "Arduino.h"
#include "ElapsedTimer.h"

ElapsedTimer::ElapsedTimer(void)
{
  elapsed_seconds = 0;
  elapsed_microseconds = 0;
  last_clock = micros();
}

// Read the current system clock and count the time elapsed since the last
// update.  This will correctly handle rollover of the microsecond clock as long
// as this is called once per complete cycle (e.g. once per 70 minutes). 

void ElapsedTimer::update(void)
{
  unsigned long now = micros();

  // The following will work even if the timer rolls over given the properties
  // of two's complement arithmetic.  now and last_clock are both unsigned, but
  // large unsigned numbers are equivalent to negative numbers in a signed
  // interpretation.  E.g., if just past a rollover now==1 and
  // last_clock==0xffffffff, now - last_clock is the same as 1 - (-1) which is the
  // correct value 2.
  elapsed_microseconds += now - last_clock;  

  // Save the time at which the update occurred.
  last_clock = now;

  // If the microseconds field has overflowed, update the seconds field.
  if (elapsed_microseconds > 999999) {

    if (elapsed_microseconds <  2000000) {  // optimize the common case 
	elapsed_seconds++;
	elapsed_microseconds -= 1000000;

      } else {
	elapsed_seconds     += elapsed_microseconds / 1000000;
	elapsed_microseconds = elapsed_microseconds % 1000000;
      }
  }
}

long ElapsedTimer::elapsedMicroseconds(void)
{
  update();
  if (elapsed_seconds == 0) return elapsed_microseconds;
  else return elapsed_seconds * 1000000 + elapsed_microseconds;
}

long ElapsedTimer::elapsedMilliseconds(void)
{
  // This isn't very efficient since the current implementation works only in
  // microseconds.  A better solution would be to subclass ElapsedTimer to
  // provide both micro- and milli- second resolution implementations for use
  // based on the needed precision.
  update();
  if (elapsed_seconds == 0) return elapsed_microseconds / 1000;
  else                      return elapsed_seconds * 1000 + elapsed_microseconds / 1000;
}

long ElapsedTimer::elapsedSeconds(void)
{
  update();
  return elapsed_seconds;
}

void ElapsedTimer::reset(void)
{
  elapsed_microseconds = 0;
  elapsed_seconds = 0;
  last_clock = micros();
}
void ElapsedTimer::subtractMicroseconds(long interval)
{
  elapsed_microseconds -= interval;

  // If the microseconds field has underflowed, adjust the seconds field.
  if ( elapsed_microseconds < 0 ) {
      if (elapsed_microseconds > -2000000) {  // optimize the common case 
	elapsed_seconds--;
	elapsed_microseconds += 1000000;

      } else {
	elapsed_seconds     += elapsed_microseconds / 1000000;
	elapsed_microseconds = elapsed_microseconds % 1000000;
      }
    }
}

void ElapsedTimer::delayUntilElapsedMicroseconds( unsigned long interval )
{
  while (elapsedMicroseconds() < interval);
}
