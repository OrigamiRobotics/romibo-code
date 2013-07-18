// CommandLine.pde : simple command processor for interactive debugging and control
// Copyright (c) 2011-2012 Garth Zeglin

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
#include <ConsoleOutput.h>
#include <Parameters.h>
#include <HardwareSerial.h>
#include <RomiboAction.h>

extern ConsoleOutput Console;
extern Parameters parameters;

static int execute_command_count;
static int current_count;

void print_matching_variable( const char *name, int value )
{
  Console.print(name);
  Console.print(" = ");
  Console.println(value);
}

// Process a command string which has been parsed into tokens.  This is not a
// very efficient interpreter, but simple to code and maintain.  This is called
// from the main event loop, so it may modify global state and trigger events
// but should not stall if it is to be called during normal operation.

void execute_command( int argc, char **argv )
{
  execute_command_count++;
  if (argc > 0) {
    if (!strncmp(argv[0], "stat", 4)) {
      Console.print("\nTop photo = ");    Console.print( Romibo.topLightLevel() );
      Console.print("; Left photo = ");   Console.print( Romibo.leftFrontLightLevel() );
      Console.print("; Right photo = ");  Console.println( Romibo.rightFrontLightLevel() );
      Console.print("; Range = ");        Console.println( Romibo.frontRangeDistance() );
      Console.print("; Touch = ");        Console.println( Romibo.touchSwitches() ); 
    } 

    else if (!strcmp(argv[0], "get" )) {
      // Print all variables with initial string matching the given string.
      // If none is given, match everything.
      if (argc > 1)  parameters.forEachMatching( print_matching_variable, argv[1] );
      else           parameters.forEachMatching( print_matching_variable, "" );
    }

    else if (!strcmp(argv[0], "set" )) {
      if (argc < 3) Console.println("set command format: set name value");

      // Set the parameter to the given value.
      else parameters.setFirstMatching( argv[1], atoi(argv[2]));
    }
    

    /* this command is part of the host protocol */
    else if (!strcmp(argv[0], "drive" )) {
      int left = 0, right = 0;
      if (argc > 1) left = atoi(argv[1]);
      if (argc > 2) right = atoi(argv[2]);
      Romibo.drive( left, right );
    }

  else if (!strcmp(argv[0], "tilt" )) {
   
      int xtilt = 50, ytilt = 50;
      if (argc > 1) xtilt = atoi(argv[1]);
      if (argc > 2) ytilt = atoi(argv[2]);
      Romibo.setHeadPosition( ytilt, xtilt );
    }


    /* this command is part of the host protocol */
    else if (!strcmp(argv[0], "stop")) {
      Romibo.stop();
    }

    else if (!strcmp(argv[0], "count"))
    {
      current_count = (current_count < 10) ? current_count + 1 : 1;

      char countCommand[8];
      
      if (sprintf(countCommand, "%d.wav", current_count) > -1)
      {
	Console.print("counting "); Console.println(countCommand);

	Romibo.playWavFile(countCommand);
      }

    }


    /* this command is part of the host protocol */
    else if (!strcmp(argv[0], "say")) {

      const char *sound = "S1";

      if (argc > 1) {

	//current iPad app passes wav file name directly
	char* extension = ".wav";
	char* extensionUpper = ".WAV";

	if ( (strstr(argv[1], extension) != NULL) || (strstr(argv[1], extensionUpper) != NULL) )
	{
	  Romibo.playWavFile(argv[1]);
	  sound = argv[1];

	  

	  if ( (strstr(sound, "Yes_2.wav") != NULL ) ||
	       (strstr(sound, "up_and_d.wav") != NULL ))
	    {
	      Romibo.bob();
	    }

	  if ( strstr(sound, "no_1.wav") != NULL )
	    {
	      Romibo.sway();
	    }

	   if ( strstr(sound, "Spin_Aro.wav") != NULL )
	    {
	      Romibo.spinInPlace(100);
	    }

	   if ( (strstr(sound, "right_an.wav") != NULL ) ||
	        (strstr(sound, "side_to_.wav") != NULL ) )
	    {
	      Romibo.sway();
	    }

	   if ( strstr(sound, "Spin_and.wav") != NULL )
	    {
	      Romibo.spinInPlace(100);
	      Romibo.bob();
	    }

	   if ( strstr(sound, "Head_Rol.wav") != NULL )
	    {
	      Romibo.sway();
	      Romibo.bob();

	      //Romibo.headRoll();
	    }




	}

	else
	{
	  // map the abstract sound names to specific sound files
	  // used in early versions of the iPad app
	  if      (!strcmp( argv[1], "WHISTLE" )) sound = "BECKON1";
	  else if (!strcmp( argv[1], "GROWL" ))   sound = "ANGRY1";
	  else if (!strcmp( argv[1], "PLAY" ))    sound = "PLAYWME";
	  else if (!strcmp( argv[1], "BYE" ))     sound = "GOODBYE";

	  Romibo.playSoundNamed( sound );
        }

	Console.print("Playing sound: "); Console.println(sound);

      }

    }

    /* this command is part of the host protocol */
    else if (!strcmp(argv[0], "antenna")) {
      int color[3] = { 255, 255, 255 };
      if (argc > 1) color[0] = atoi( argv[1] );
      if (argc > 2) color[1] = atoi( argv[2] );
      if (argc > 3) color[2] = atoi( argv[3] );
      Romibo.setAntennaColor(color);
    }

    /****************************************************************/

    else if (!strcmp(argv[0], "look_down"))
      {
	Romibo.tiltHeadForward();
      }

    else if (!strcmp(argv[0], "look_up"))
      {
	Romibo.tiltHeadBack();
      }

    else if (!strcmp(argv[0], "eye")) {
      int eye = 50;
      if (argc > 1) eye = atoi( argv[1] );
      Romibo.setEyelid( eye );
    }

    else if (!strncmp(argv[0], "forward", 3)) {
      int speed = 100;
      if (argc > 1) speed = atoi(argv[1]);
      Romibo.drive( speed, speed );
    }

    else if (!strcmp(argv[0], "rev")) {
      int speed = 100;
      if (argc > 1) speed = atoi(argv[1]);
      Romibo.drive( -speed, -speed );
    }

    else if (!strcmp(argv[0], "rt")) {
      int speed = 100;
      if (argc > 1) speed = atoi(argv[1]);
      Romibo.drive( Romibo.leftSpeed(), speed );
    }

    else if (!strcmp(argv[0], "lt")) {
      int speed = 100;
      if (argc > 1) speed = atoi(argv[1]);
      Romibo.drive( speed, Romibo.rightSpeed() );
    }

    else if (!strcmp(argv[0], "rot")) {
      int speed = 100;
      if (argc > 1) speed = atoi(argv[1]);
      Romibo.drive( speed, -speed );
    }
	
	else if (!strcmp(argv[0], "spin")) {
	  int speed = 100;
	  if (argc > 1) speed = atoi(argv[1]);
	  Console.println("spinning");
	  Romibo.drive( speed, -speed );
    }
	
	else if (!strcmp(argv[0], "bob")) {
      Romibo.setHeadPosition (0,0);
	  Romibo.delay (300);
	  Romibo.setHeadPosition (100,100);
	  Romibo.delay (300);
	  Romibo.setNeutralHeadPosition ();
    }
	
	else if (!strcmp(argv[0], "rock")) {
      Romibo.setHeadPosition (0,100);
	  Romibo.delay (400);
	  Romibo.setHeadPosition (100,0);
	  Romibo.delay (400);
	  Romibo.setNeutralHeadPosition ();
    }

	else if (!strcmp(argv[0], "turn")) {
      int speed = 100;
      if (argc > 1) speed = atoi(argv[1]);
      Romibo.drive( 0, speed );
    }

	else if (!strcmp(argv[0], "sway"))
	  {
	    Romibo.sway();
	  }
	
	else if (!strcmp(argv[0], "nod")) {
      Romibo.setHeadPosition (0,100);
	  Romibo.delay (250);
	  Romibo.setHeadPosition (100,0);
	  Romibo.delay (250);
      Romibo.setHeadPosition (0,100);
	  Romibo.delay (250);
	  Romibo.setHeadPosition (100,0);
	  Romibo.delay (250);
	  Romibo.setNeutralHeadPosition ();
	}
	
	else if (!strcmp(argv[0], "dance")) {
	  Romibo.playSoundNamed("HAPPY1");
	  RomiboAct.setAntenna (ROA_BLUE);
	  Romibo.drive(50,-50);
	  Romibo.delay(500);
	  Romibo.drive(0,0);
	  Romibo.setHeadPosition (0, 50);

	  Romibo.drive(-50,50);
	  Romibo.delay(500);
	  Romibo.drive(0,0);
	  //Romibo.setHeadPosition (0, 50);
	  

	  Romibo.drive(50,-50);
	  Romibo.delay(500);
	  Romibo.drive(0,0);
	  //Romibo.setHeadPosition (100, 50);


	  Romibo.drive(-50,50);
	  Romibo.delay(500);
	  Romibo.drive(0,0);

	  Romibo.setNeutralHeadPosition();
	 
	  Romibo.playSoundNamed ("HAPPY3"); 
	}

	else if (!strcmp(argv[0], "emote" )) 
	  {

	  int x = 0, y = 0;
	  int absx, absy;
	 
	  if (argc > 1) x = atoi(argv[1]);
	  if (argc > 2) y = atoi(argv[2]);
	  
	  absx = abs(x);
	  absy = abs(y);

	  /*	  if (x < 0) absx = -1 * x;
	  else (absx = x);
	  
	  if (y < 0) absy = -1 * y;
	  else (absy = y);
	  */

	  if (x > 0) 
	  {
	    if (y < 0)
	      {
	
		//satisfied
		Romibo.setAntennaColorYellow();
		Romibo.playSoundNamed ("BECKON2");
		Console.println("satisfied");
		
	      }
	    else
	      {
		//excited
		Romibo.setAntennaColorGreen();
		Romibo.playSoundNamed ("HAPPY1");
		Console.println("happy");

	
		
	      }
	  } 
	  else 
	  {
	    if (y < 0)
	      {
		
		//sad
		Romibo.setAntennaColorBlue();
		Romibo.playSoundNamed ("SAD3");
		Console.println("sad");
	      }
	    else
	      {
		RomiboAct.setAntenna(ROA_RED);
		Romibo.playSoundNamed ("ANGRY1");
		Console.println("angry");

	
	      }
	  } 
    }

    else {
      Console.print("Command not recognized: ");
      Console.println( argv[0] );
    }
  }
}

// returns the number of calls to execute_command
int get_command_count (void)
{
  return execute_command_count;
}

/****************************************************************/
int parse_arg_line(char *string, char **argv, int argmax)
{
  int argc = 0;
  char *cp = string;
  while (*cp && argc < argmax) {
    while (*cp && isspace(*cp)) cp++;     /* scan past white space */
    if (*cp) argv[argc++] = cp;           /* save pointer to text */  
    else break;
    while (*cp && !isspace(*cp)) cp++;    /* scan past text */
    if (*cp) *cp++ = 0;
  }
  return argc;
}

/****************************************************************/
// Define a simple global buffer for accumulating a command string.
const int max_command_length = 80;
const int max_command_terms  =  5;
static char command_buffer[max_command_length+1];
static int next_command_char = 0;

// Polling function to be called from the main event loop.
void poll_command_input(void)
{
  while (Serial.available()) {
    int next_char = Serial.read();

    // once a line terminator is received, process the available command string
    if (next_char == '\r') {
      command_buffer[next_command_char] = 0; // terminate the string

      // parse the string into pieces and execute them
      char *argv[max_command_terms];
      int argc = parse_arg_line( command_buffer, argv, max_command_terms);
      execute_command(argc, argv);

      // then reset the command buffer
      next_command_char = 0;
    }

    // else store the characters until a full line is received, ignoring overrun
    if (next_command_char < max_command_length) {
      command_buffer[next_command_char++] = next_char;
    }
  }
}
/****************************************************************/
#if ROMIBO_WIFLY_INSTALLED
// This should be refactored with the previous function.
// Polling function to be called from the main event loop.
void poll_network_input(void)
{
  static char command_buffer[max_command_length+1];
  static int next_command_char = 0;
  static int connected = 0;

  while (Serial1.available()) {
    int next_char = Serial1.read();

    // once a line terminator is received, process the available command string
    if (next_char == '\r') {
      command_buffer[next_command_char] = 0; // terminate the string

      Console.print("received network command: ");
      Console.println( command_buffer );

      // parse the string into pieces and execute them
      char *argv[max_command_terms];
      int argc = parse_arg_line( command_buffer, argv, max_command_terms);

      // a little logic to ignore handshaking output from the WiFly until the stream is established
      if (!connected) {
	if (argc > 0 && !strcmp( argv[0], "begin")) connected = 1;

      } else {
	if (argc > 0 && !strcmp( argv[0], "end")) connected = 0;
	else execute_command(argc, argv);
      }

      // then reset the command buffer
      next_command_char = 0;
    }

    // else store the characters until a full line is received, ignoring overrun
    if (next_command_char < max_command_length) {
      command_buffer[next_command_char++] = next_char;
    }
  }
}

#endif
/****************************************************************/
