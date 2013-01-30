Welcome to the Romibo Project!

This folder contains the source code for the standard programs which run on the
Romibo robot.  The Romibo uses an Arduino microcontroller and this folder
follows the Arduino conventions to make it easy to get started.  In the simplest
case, you can simply set this folder to be the "Sketchbook location" within the
preferences of the Arduino application.  Then you can load the Romibo sketch
from the Sketchbook and get started.

These components are provided:

   Romibo           the default Arduino "sketch" for the Romibo character
   RomiboExample1   a minimal sketch to demonstrate compiling, loading, and remote control

   libraries/       a folder for holding common code
     RomiboRobot    the common hardware interface for operating the robot
     RomiboSound    a library for playing sounds from an SD card

You'll also need to install the Arduino application (version 1.0 or later).  If
this is your only Arduino project, you can simply set this folder as the
Sketchbook location.  Otherwise, you may move these components into the
corresponding locations of your existing sketchbook folder.

Once you start the Arduino application and load the Romibo sketch, you will need
to tell it what kind of hardware is used in the robot.  Please find the Board
menu under Tools and select Arduino Mega 2560.  Then you should be able to
compile the code with the "verify" operation. On a Mac this is invoked with
either cmd-R, the check button, or from the Sketch menu.  This converts the
source code into object code which can actually run on the robot.

If you prefer, you can do these steps before you connect the robot.  To actually
install and run the program, you'll need to connect a USB cable to the robot,
then configure the Serial Port setting on the Tools menu to tell the application
where to send the object code.  The exact name will vary depending upon your
configuration.  If you set the correct port, the upload operation should
reprogram the robot.

The standard Romibo code includes a simple command line shell to help debug
programs and hardware.  Invoke the Serial Monitor brings up a window which
display output from the robot and provides a text box from which you can send
commands to the robot.  This only works while the USB cable is connected, but it
a useful way to debug a program before letting the robot roam disconnected.


