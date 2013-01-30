// ElapsedTimer.h : measure time intervals
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

// Define a software timer to measure intervals using the underlying system
// clock.  This is mostly just a bit of bookkeeping, but provides a simple API
// to hide limitations and overflow of the system clock and which can scale to
// longer intervals should they be needed.

#ifndef __ELAPSEDTIMER_H_INCLUDED__
#define __ELAPSEDTIMER_H_INCLUDED__

class ElapsedTimer
{
protected:
  long elapsed_seconds;
  long elapsed_microseconds;
  unsigned long last_clock;
  void update(void);

public:
  // Default constructor which resets the timer.
  ElapsedTimer(void);

  // Reset the elapsed time to zero.
  void reset(void);

  // Return the number of microseconds elapsed since time zero.
  long elapsedMicroseconds(void);

  // Return the number of milliseconds elapsed since time zero.
  long elapsedMilliseconds(void);

  // Return the number of seconds elapsed since time zero.
  long elapsedSeconds(void);

  // A reliable delay function.  (The defaultdelayMicroseconds() can only handle
  // 16.384ms sleeps).  Delay until the time elapsed since reset exceeds the
  // specified interval, possibly not waiting at all.  This could be extended to
  // use delay() and delayMicroseconds(), but for now just busywaits calling the
  // micros() function.
  void delayUntilElapsedMicroseconds( unsigned long interval );

  // Subtract the given interval from the elapsed time.  Note that this amount
  // can exceed the current elapsed time, putting the time origin into the
  // future and causing the timer to return negative values.
  void subtractMicroseconds(long interval);

};

#endif // __ELAPSEDTIMER_H_INCLUDED__
