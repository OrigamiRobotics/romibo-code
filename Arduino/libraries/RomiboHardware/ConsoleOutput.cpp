// ConsoleOutput.cpp : buffered, polled serial output stream
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
#include "HardwareSerial.h"
#include "ConsoleOutput.h"

ConsoleOutput::ConsoleOutput(void)
{
  // Define the buffer size; it can hold buffer_size-1 characters.
  buffer_size = 2000;

  // Allocate the buffer, noting but ignoring any failure. 
  buffer = (char *) malloc( buffer_size );
  if (buffer == NULL) Serial.println("WARNING: ConsoleOutput buffer allocation failed.");

  // Initialize ring pointers.
  data = space = 0;
}

/****************************************************************/
// Ring buffer functions.  This should probably be moved into an abstract class.

// Return the number of bytes of free space available in the ring buffer.
unsigned ConsoleOutput::space_available( void )
{
  int diff = buffer_size + space - data;
  while (diff >= buffer_size) diff -= buffer_size;
  return buffer_size - 1 - diff;
}

void ConsoleOutput::buffer_data_added( unsigned bytes )
{
  unsigned new_space = space + bytes;
  // Wrap the index around without using the modulo operator.
  while (new_space >= buffer_size) new_space -= buffer_size;
  space = new_space;
}

void ConsoleOutput::buffer_data_removed( unsigned bytes )
{
  unsigned new_data = data + bytes;
  while (new_data >= buffer_size) new_data -= buffer_size;
  data = new_data;
}

/****************************************************************/
// This should be called from the main event loop to service the output.
void ConsoleOutput::poll(void)
{
  // Check if the buffer has data to send and the transmitter is available.
  if (data != space && isUARTWriteReady()) {
    Serial.write(buffer[data]);
    buffer_data_removed(1);
  }
}

/****************************************************************/
// The default Arduino HardwareSerial library does not include a function to
// check that the serial transmitter is available, so it isn't usually possible
// to perform non-blocking output.  This function is hard-coded to check whether
// UART0 can accept another character without blocking.
int ConsoleOutput::isUARTWriteReady(void)
{
  return (UCSR0A) & (1 << UDRE0);
}

/****************************************************************/
size_t ConsoleOutput::write( uint8_t c)
{
  // If there is no buffer space available, block while attempting to transmit
  // data until a space clears.
  while ( space_available() == 0) poll();

  buffer[space] = c;
  buffer_data_added(1);
  return 1;
}
/****************************************************************/
