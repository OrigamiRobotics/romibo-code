// RomiboRobot.h : Control API for the Romibo robot.
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

// The essence of a good API is provide a convenient means of creating behaviors
// while hiding the implementation details which are likely to change.

// For the robot, this includes:
//
//  Providing motion commands using calibrated units of position and
//  time.  This decouples the program from the specific hardware.
//
//  Filtering sensor inputs: debouncing switch inputs, smoothing analog inputs.
//  Sensors should reported in calibrated or abstract units.
//
//  Storing calibration data to map sensor and command values to the raw
//  hardware values.  Storing threshold values to define critical conditions.
//
//  Providing a standard means to configure the library for different hardware
//  versions.
//
//  Hiding the state machine if desired.  The library should support both
//  scripting with sequential procedures (easier for novices) and polling from a
//  state machine.  This can be accomplished if all polling and robot state
//  update can be hidden within API calls.  This is already half-heartedly
//  assumed: some of the existing code is using delay(), which breaks any hope
//  of systematic I/O updates or sensor filtering.
//
//  Protecting against invalid inputs.  In all cases, values out of range should
//  be clipped to the nearest reasonable value.
//
//  Additionally, we will eventually want to manage some common primitives
//  directly from within the event loop, e.g. smooth servo transitions, motor
//  speed ramps, or periodic movements.

// Other thoughts:

//  I have specifically kept the API calls oriented around the physical state.
//  Emotional modeling should be up to the user, e.g., I don't think this should
//  include calls like actSad() or actHappy().

/****************************************************************/
#ifndef __ROMIBOROBOT_H_INCLUDED__ 
#define __ROMIBOROBOT_H_INCLUDED__ 

#include <stdbool.h>

#include "RomiboConfig.h"
#include "RomiboServo.h"
#include "ElapsedTimer.h"
#include "Parameters.h"

enum romibo_proximity_t { ROMIBO_CLOSE, ROMIBO_MEDIUM, ROMIBO_FAR };
enum romibo_brightness_t { ROMIBO_DARK,  ROMIBO_DIM, ROMIBO_BRIGHT };

/****************************************************************/
class RomiboRobot
{
  // The protected variables and functions are hidden from the user to allow changing
  // the implemention behind the public interface.

protected:

  // A function to iterate through all polling callbacks and execute each one
  void execute_poll_cb (void);
  
  // A microsecond timer for poll() to use in measuring when next to run an I/O cycle.
  ElapsedTimer pollTimer;
  unsigned cycle_count;
  int overruns;          // count of how many polling cycles experienced significant time overrun

  // Define a C structure type to hold all values related to an analog sensor channel.
  struct romibo_sensor_t {
    // these all use raw units for higher precision 
    int raw, smoothed, average, min, max;

    // these currently range on [0, 255]
    int thresh1, thresh2;
    
    // the normalized value
    int scaled;

  } photo_left, photo_right, photo_top, range, microphone;

  // utility functions for sensor processing
  static void update_sensor_filter( struct romibo_sensor_t *sensor, int recompute_bias );
  static void init_sensor_struct( struct romibo_sensor_t *sensor );
  static int  discretized_sensor_value( struct romibo_sensor_t *sensor );

  // State of the servo drive system.  Some of these variables should probably
  // be put into a servo_var_t struct, at least if we add trajectory tracking.

  // Servo limits, specified in output angles for the servo write() function.
  int front_max;   // position closest to the neck
  int front_min;   // position closest to the head structure
  int back_min ;   // position closest to the neck
  int back_max ;   // position closest to the head structure
  int eye_min  ;   // most wide-open position
  int eye_max  ;   // most closed position

  // Most recently commanded positions.
  int front_pos, back_pos, eye_pos;

  // Objects representing the servo PWM output.
  Servo tilt_servo[3];

  // State of the wheel drive system.  This might eventually expand to support
  // trapezoidal profiles for acceleration and deceleration control.
  int left_speed, right_speed;

  // A descriptor table for the singleton Romibo object which is added to the
  // global parameter set to allow interactive debugging at runtime.  This is
  // declared as a static class member so it has access to the protected and
  // private data in the Romibo object.
  static struct parameter_descriptor_t parameter_descriptor_table[];
  
  // Most recent button states
  int top_switch;
  int left_switch;
  int right_switch;
  int bottom_switch;
  int switches;

  /****************************************************************/
public:
  // default constructor, used to create a statically allocated singleton object named Romibo
  RomiboRobot();

  // Initialize the actual hardware.
  void begin(void);

  // Access to the metadata for the Romibo singleton object.
  static struct parameter_descriptor_t *parameters_table(void);
  static size_t parameters_table_size(void);

  // ------------ Sensor inputs -----------------------------------------------

  // The poll() method is called internally by API methods to check whether it
  // is time to sample the sensors, update the outputs, recompute trajectories,
  // update automatic calibration, pump serial output, process console commands,
  // etc.  All I/O updates should occur at a constant rate (e.g. 100Hz).  It may
  // also be called by the user to guarantee that I/O is up to date.
  void poll(void);
  
  // Allows access to polling cycle by adding a callback. The added function 
  // will be called every time poll() is called internally. As a result, loops,
  // long callbacks, or blocking calls such as calls to RomiboRobot::delay, 
  // sleep, etc should be avoided.
  // returns an id for the callback which can be used to remove it.
  // if multiple callbacks are added, there is on guarantee on the order of execution.
  int add_polling_callback(void (*fun) ());
  
  // Removes the polling callback corresponding to the id.
  // Returns 0 on success, -1 if id not found.
  int remove_polling_callback(int id);
  
  // Removes all polling callbacks.
  void reset_polling_callbacks (void);
  
  // Allows user to set a callback for button switches. The set function
  // will be called every time a button changes from non-pressed to pressed state
  // The callback should take an unsigned argument which represents the button state,
  // equivalent to the return value of RomiboRobot.touchSwitches()
  void set_button_callback(void (*fun) (unsigned));
  
  // Resets the button callback to a null function
  void reset_button_callback ();

  // Return the number of sensor sampling cycles, which can be used as a low-precision clock.
  unsigned sensorSamples(void);

  // Returns an integer on [0, 1000] with the number of millimeters of clear
  // space (e.g. distance to the nearest obstacle.)  If nothing is in front,
  // report the maximum distance 1000.  Note that the value may be quite
  // approximate, i.e., we could do some basic interpolated calibration but not
  // promise high precision.  The value should, however, increase monotonically
  // with distance even if not linear.  These values may be filtered to reduce
  // noise.
  int frontRangeDistance(void); 
  
  // Return an integer on [0, 100] with the percentage of light falling on the
  // given sensor.  Zero represents complete darkness, 100 full brightness.  The
  // value may not be linear but should increase monotonically.  These values
  // are filtered to reduce noise.
  int topLightLevel(void);
  int leftFrontLightLevel(void);
  int rightFrontLightLevel(void);

  // Return sensor values binned into three levels (close, medium, far) or
  // (dark, dim, bright), encoded as (0, 1, 2).  The bins are automatically
  // adjusted based on the minimum and maximum values observed.
  int obstacleProximity(void);
  int topBrightness(void);
  int leftBrightness(void);
  int rightBrightness(void);

  // Convenience functions to return a heading value estimating the angle of the
  // light in front.  The result is mapped over [-90, 90] to indicate the turn
  // angle in degrees which would face the light.
  int frontLightHeading(void);

  // Return true if the given switch is pressed.
  int touchSwitch(int switchnum);
  int topSwitch(void);
  int bottomSwitch(void);
  int leftSwitch(void);
  int rightSwitch(void);

  // Return an integer of debounced touch switch states, one per bit.  If zero,
  // nothing is pressed, any other combination of bits represents some contact.
  unsigned touchSwitches(void);

  // Return an integer on [0, 100] with the ambient sound level.
  int soundLevel(void);

  // The remote control IR signal should really be sampled on a timer interrupt,
  // it isn't possible to stall the whole machine waiting for a button to be
  // pressed.

  // Report the most recent IR remote command key, returning zero if nothing new
  // has been received since the last call.
  int getRemoteControlKey(void);

  // Report the current state of the motor outputs.
  int leftSpeed(void);
  int rightSpeed(void);

  // ----------- Action controls ------------------------------------------------

  // Speed is an integer on [-100,100] representing the percentage of 'full
  // speed'.  E.g. output = map( speed, -100, 100, speed_min, speed_max ); The
  // actual motor state may be an approximation of this given that limitations
  // of PWM generation, but that could change in the future.
  void drive(int leftSpeed, int rightSpeed);

  // Following are some convenience functions that will capture user intent
  // better.  There is some editorial discretion about how many of these should
  // be provided, since they are strictly redundant.
  void stop(void);
  void driveForward(int speed);
  void driveBackward(int speed);
  void spinInPlace(int speed);    // speed on [100, -100] maps to [full CCW, full CW] rotation rate

  // A shim to ease the translation of older code.  This should be phased out of
  // the public interface.  The motor index is either ROMIBO_LEFT_MOTOR or
  // ROMIBO_RIGHT_MOTOR.  dir is positive for forward, else reverse.  spd is a
  // positive speed value on [0, 255].
  void setDriveMotor( int mot, int dir, int spd );

  // Choose motor speeds to aim toward the light in front.
  void driveTowardLight(int speed);

  // Set the eyelid position.  The position is an integer on [0, 100]
  // representing the percentage the eye is "open".
  void setEyelid(int eye_open_position);

  // Set the neck actuators.  Each position is an integer on [0, 100]
  // representing vertical position: {0,0} is as scrunched down as possible,
  // {100, 0} is leaning as far back as possible, etc.  Overloaded forms are
  // provided for either component angles or a position array.
  void setHeadPosition( int front_position, int back_position );
  void setHeadPosition( int neck_positions[2] );

  // Convenience functions to move the head.
  void setNeutralHeadPosition(void);
  void tiltHeadForward(void);
  void tiltHeadBack(void);

  // Each color channel is specified as an integer on [0,255] representing
  // intensity.  The actual implementation may or may not offer proportional
  // color.  Several overloaded forms are provided to either specify an entire color
  // or components.
  void setAntennaColor( uint8_t color[3] );     // color array is ordered as {red, green, blue};
  void setAntennaColor( uint8_t red, uint8_t green, uint8_t blue );
  void setAntennaColor( int color[3] );

  // These sleep functions continue to process robot I/O during the sleep
  // interval, and must be used instead of the Arduino equivalents for I/O
  // filtering is to run properly.
  void delay(unsigned long milliseconds_duration);
  void delayMicroseconds(unsigned long microseconds_duration);

  // Synchronize to the sensor input cycle.  This briefly pauses until the
  // values for the sensors have been re-sampled and the filters updated.
  void waitForNewSensorData(void);

  // Control the audio output.  Currently starting a sound takes a long time
  // during which no inputs will be read or outputs changed.
  void playSoundNamed(const char *name);  // Given an 8-character base filename, play the corresponding .WAV file.
  int  isSoundPlaying(void);              // Returns true if a sound is currently playing.
  void stopSound(void);
  void waitForSoundToFinish(void);        // Pauses until the current sound is complete.
  bool soundIsEnabled(void);             // Returns true if the sound system is present and working.

  // The following would eventually make sense to implement, though perhaps not right away.
  // Or they may be better placed in a RomiboBehavior library.
#if 0

  // Following are low-level servo or trajectory modes which can be enabled.
  // These are provided as primitives to simply user code.  They will require
  // internal state variables to be updated during polling.
  void bounceHeadUpAndDown(int speed, int cycles);

  // Rotate the body for the approximate change in heading.  This is
  // synchronous, it doesn't return until the turn is done.
  void turnInPlaceForAngle( int heading );

  // Some users may want access to the internal event loop, so we may eventually
  // want callbacks to indicate events.  This might normally be implemented by
  // subclassing, but the Arduino style of static declaration makes that more
  // awkward, and this is more compatible with C-style top-level programming.
  typedef void (*handler_function_t)(void); 
  void whenSoundEnds( handler_function_t handler);
  void whenTouched( handler_function_t handler );
  void whenIdle( handler_function_t handler );
  void setPeriodicTimer( handler_function_t handler_function_t, int milliseconds_interval );
#endif

};
//declarations for functions implemented in commandline.cpp
void poll_command_input(void);
int get_command_count (void);
#if ROMIBO_WIFLY_INSTALLED
void poll_network_input(void);
#endif


/****************************************************************/
// Global object for accessing the robot hardware.
extern RomiboRobot Romibo;   

/****************************************************************/
#endif // __ROMIBOROBOT_H_INCLUDED__ 

