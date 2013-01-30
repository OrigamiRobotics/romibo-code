// ConsoleOutput.h : buffered, polled serial output stream
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
//
// The standard implementation of Serial provided with the Arduino has
// unbuffered output, so the event loop will pause while characters are being
// sent out.  This object provides a buffered output on top of the standard
// Serial object.

#ifndef __CONSOLEOUTPUT_H_INCLUDED__
#define __CONSOLEOUTPUT_H_INCLUDED__

#include "Print.h"

class ConsoleOutput : public Print
{
private:
  // Ring buffer to accumulate output.
  char *buffer;        // character ring buffer  
  int buffer_size;     // number of bytes in the buffer, the capacity is one less
  
  // Ring buffer indicators.  When data == space the buffer is empty.  The buffer is full when holding (buffer_size-1) bytes.
  unsigned data;             // Position of first byte of data.
  unsigned space;            // Position of first byte of empty space.

  // Ring buffer bookkeeping.
  void buffer_data_added( unsigned bytes );
  void buffer_data_removed( unsigned bytes );

  // Check whether the underlying serial device can accept another byte.
  int isUARTWriteReady(void);

public:
  ConsoleOutput(void);

  // The polling function should be called frequently from the main event loop
  // to pump data into the serial output device.
  void poll(void);

  // Returns the number of bytes the output buffer can receive without overflowing.
  unsigned space_available( void );

  // Provide the primitive function required by the Print class to implement
  // output.  Note that this will block if the buffer fills up, so maintaining
  // critical timing still requires the user to limit the overall data output
  // rate.
  size_t write( uint8_t c);
};

#endif // __CONSOLEOUTPUT_H_INCLUDED__
