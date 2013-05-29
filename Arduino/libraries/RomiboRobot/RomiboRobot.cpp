// RomiboRobot.cpp : Control API for the Romibo robot.
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Arduino.h"
#include <RomiboSound.h>  // Include the customized WaveHC sound library.
#include "RomiboServo.h"  // Include the customized servo functionality library.

#include "RomiboRobot.h"
#include "ConsoleOutput.h"
#include "RomiboPinMappings.h"

#include <MMA8453_n0m1.h>   // Include library for the accelerometer

/****************************************************************/
// Time constants.
static const int pollInterval = 10000;  // in microseconds
static const int biasInterval = 64;     // in sampling intervals

/****************************************************************/
// Provide an initial compatibility implementation which just uses existing
// global objects.  This will allow a smooth transition to using private
// objects.

extern ConsoleOutput Console;

/****************************************************************/
// Perform basic filtering on an analog input signal.
void RomiboRobot::update_sensor_filter( struct romibo_sensor_t *sensor, int recompute_bias )
{
    // Apply a basic integer smoothing filter.
    sensor->smoothed += (sensor->raw - sensor->smoothed) >> 4;

    // Keep track of absolute ranges of the min and max values.  These might
    // eventually incorporate a decay to reduce sensitivity to spurious values.
    if (sensor->smoothed > sensor->max) sensor->max = sensor->smoothed;
    if (sensor->smoothed < sensor->min) sensor->min = sensor->smoothed;

    // apply a long-term averaging filter to get a DC bias
    if (recompute_bias) {
        int error = sensor->raw - sensor->average;
        int feedback = error >> 3;

        if ( feedback == 0 ) {  // protect against truncation underflow
            if ( error < 0 )      feedback = -1;
            else if ( error > 0 ) feedback = 1;
        }
        sensor->average += feedback;
    }
}

void RomiboRobot::updateAccVector( Accelerometer* acc) {
    (acc->acc)->update();
    (acc->vec)->x = (acc->acc)->x();
    (acc->vec)->y = (acc->acc)->y();
    (acc->vec)->z = (acc->acc)->z();
}

/****************************************************************/

// A linked list to store callbacks executed during poll()
typedef struct pollcbnode_t {
    int id; // the id of the callback
    void (*fun) (void); // the function to be called
    struct pollcbnode_t *next; // the next callback node in the list
} pollcbnode;

pollcbnode *poll_callback = 0;

static int nextpollcbid = 1;

void RomiboRobot::execute_poll_cb (void) {
    if (poll_callback == 0) return;

    pollcbnode *curr = poll_callback;

    while (curr != 0) {
        curr->fun();
        curr = curr->next;
    }
}

// stores the button callback
void (*button_callback) (unsigned) = 0;

void RomiboRobot::set_button_callback(void (*fun) (unsigned)) {
    button_callback = fun;
}

// Resets the button callback to a null function
void RomiboRobot::reset_button_callback () {
    button_callback = 0;
}

void RomiboRobot::poll(void) 
{
    unsigned long elapsed = pollTimer.elapsedMicroseconds();
    if (elapsed > pollInterval) { 

        // If the interval is close to the normal interval, then subtract a fixed
        // amount from the polling clock.  This will keep the average sampling rate
        // constant, since if one period runs a little long, the timer will fire a
        // little sooner on the next cycle.  However, if some action took
        // substantially longer, then just reset the clock to avoid multiple iring
        // in rapid succession.
        if (elapsed < 2*pollInterval) pollTimer.subtractMicroseconds( pollInterval );
        else {
            pollTimer.subtractMicroseconds( elapsed );
            overruns++;
        }

        // Call the poll callback, if applicable
        execute_poll_cb ();

        // Count sensor samples.
        cycle_count++;

        // Now sample all the analog sensors.

        // Sample the main board accelerometer
        updateAccVector(accel_mobo);
        // Sample the head breakout board accelerometer
        updateAccVector(accel_hbbo);

        // Brighter light == lower resistance == lower voltage reading, so invert the sense.
        photo_left.raw  = 1023 - analogRead( photo_sensor[0] );
        photo_right.raw = 1023 - analogRead( photo_sensor[1] );
        //photo_top.raw   = 1023 - analogRead( photo_sensor[2] );

        // Farther obstacle distance == lower voltage.
        range.raw = 1023 - analogRead( range_finder );

        // Apply the standard input filters, periodically recomputing the constant DC bias.
        int recompute_bias = ((cycle_count % biasInterval) == 0);
        update_sensor_filter( &photo_left, recompute_bias );
        update_sensor_filter( &photo_right, recompute_bias );
        update_sensor_filter( &photo_top, recompute_bias );
        update_sensor_filter( &range, recompute_bias );

        // Apply the calibration scaling.  This is intended to map the arbitrary A/D
        // converter scale to something closer to stable units.  These gain factors are a
        // property of the CdS cell and bias resistor component values.
        photo_left.scaled  = constrain( map( photo_left.smoothed,  0, 700, 0,  100 ), 0, 100);
        photo_right.scaled = constrain( map( photo_right.smoothed, 0, 700, 0,  100 ), 0, 100);
        photo_top.scaled   = constrain( map( photo_top.smoothed,   0, 700, 0,  100 ), 0, 100);

        // The Sharp rangefinders are hardly linear, this is very approximate.  They
        // also alias very close distances, reporting them as farther away than the
        // minimum sensing distance.
        range.scaled = constrain( map( range.smoothed, 500, 800, 100, 300 ), 0, 1000);

        int old_switches = switches;
        top_switch = touchSwitch(ROMIBO_TOUCH_TOP);
        left_switch = touchSwitch(ROMIBO_TOUCH_LEFT);
        right_switch = touchSwitch(ROMIBO_TOUCH_RIGHT);
        bottom_switch = touchSwitch(ROMIBO_TOUCH_BOTTOM);
        switches = top_switch | (left_switch << 1)
            | (right_switch << 2) | (bottom_switch << 3);

        if (old_switches == 0 && old_switches != switches) {
            // call button callback
            if (button_callback) {
                button_callback (switches);
            }
        }

#if ROMIBO_MICROPHONE_DEFINED
        // If the microphone is present, sample it too.  Since we know we are
        // subsampling the input, this uses a different scheme to try to detect
        // peaks and estimate the approximate energy of the signal.  It also uses a
        // 0.15 fixed point scheme to use the full precision when computing the
        // average.
        microphone.raw = analogRead( microphone_pin ) << 5;
        if ( microphone.raw > microphone.max ) { microphone.max = microphone.raw; }
        if ( microphone.raw < microphone.min ) { microphone.min = microphone.raw; }
        int energy = abs(microphone.raw - microphone.average);
        microphone.average  += (microphone.raw - microphone.average) >> 8;
        microphone.smoothed += (energy - microphone.smoothed) >> 4;
        microphone.scaled = constrain( map( microphone.smoothed, 300, 2000, 0, 100), 0, 100);
#else
        microphone.scaled = 0;
#endif

    }

    // Accelerometer polling:

    

    // Send data if needed to the console output stream.
    Console.poll();

    // Check for interactive commmands.
    poll_command_input();

#if ROMIBO_WIFLY_INSTALLED
    poll_network_input();
#endif

}

int RomiboRobot::add_polling_callback(void (*fun) ())
{
    pollcbnode *newcbnode = (pollcbnode *) malloc(sizeof (pollcbnode));
    int newcbid = nextpollcbid;
    nextpollcbid ++;
    newcbnode->id = newcbid;
    newcbnode->fun = fun;
    newcbnode->next = poll_callback;

    poll_callback = newcbnode;
    return newcbnode->id;
}

int RomiboRobot::remove_polling_callback (int id)
{
    if (poll_callback == 0) return -1;

    pollcbnode *prev = 0;
    pollcbnode *curr = poll_callback;

    while (curr != 0) {
        if (curr->id == id) {
            if (prev != 0) {
                prev->next = curr->next;
            } else {
                poll_callback = 0;
            }
            free(curr);
            return 0;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    return -1;

}

void RomiboRobot::reset_polling_callbacks (void)
{
    if (poll_callback == 0) return;

    pollcbnode *curr = poll_callback;

    while (curr != 0) {
        pollcbnode *tmp = curr;
        curr = curr->next;
        free (tmp);
    }
    poll_callback = 0;
    return;
}

/****************************************************************/
void RomiboRobot::init_sensor_struct( struct romibo_sensor_t *sensor) 
{
    // fill in some sensible initial guesses to minimize startup transients
    sensor->raw      =   0;
    sensor->smoothed =   0;
    sensor->average  = 512;
    sensor->min      = 512;
    sensor->max      = 512;

    // these use an 8 bit scale, and are applied to the smoothed sensor
    // value normalized between the observed min and max limits
    sensor->thresh1  =  80;
    sensor->thresh2  = 180;
}

int RomiboRobot::discretized_sensor_value( struct romibo_sensor_t *sensor )
{
    // Map the reading from observed range of values to [0, 255] to attempt to
    // compensate automatically for sensor input limits.
    int normalized = map( sensor->smoothed, sensor->min, sensor->max, 0, 255 );
    if (normalized < sensor->thresh1) return 0;
    else if (normalized < sensor->thresh2) return 1;
    else return 2;
}


/****************************************************************/
// Default constructor.
RomiboRobot::RomiboRobot() 
{
    init_sensor_struct( &photo_left );
    init_sensor_struct( &photo_right );
    init_sensor_struct( &photo_top );
    init_sensor_struct( &range );
    init_sensor_struct( &microphone );

    // Try to initialize filters close to a steady state.
    microphone.average = 15500;

    // Configure servo calibration.
#if (ROMIBO_ELECTRONICS_MAJOR_REV==1) || (ROMIBO_ELECTRONICS_MAJOR_REV==2)
    front_max = 100;   // position closest to the neck
    front_min =  50;   // position closest to the head structure
    back_min  = 100;   // position closest to the neck
    back_max  = 150;   // position closest to the head structure
    eye_min   =  20;   // most wide-open position
    eye_max   =  90;   // most closed position
#endif
#if (ROMIBO_ELECTRONICS_MAJOR_REV==3)
    front_max = 110;   // position closest to the neck
    front_min = 0;   // position closest to the head structure
    back_min  = 80;   // position closest to the neck
    back_max  = 190;   // position closest to the head structure
    eye_min   =  60;   // most wide-open position  *** Caz *** was playing around with the eyelid calibration a bit
    eye_max   = 150;   // most closed position	*** Caz *** hard to know what these should be without a defined servo linkage length
#endif

    // The range finder is quite nonlinear.
    range.thresh1   = 160;
    range.thresh2   = 240;

    top_switch = 0;
    left_switch = 0;
    right_switch = 0;
    bottom_switch = 0;
    switches = 0;

}

/****************************************************************/
void RomiboRobot::begin(void) 
{
    // Attach to the Romibo hardware.
    // This configures the pins on the Arduino
#if (ROMIBO_ELECTRONICS_MAJOR_REV==1) || (ROMIBO_ELECTRONICS_MAJOR_REV==2)
    pinMode(mot_fwd_pin[0], OUTPUT);
    pinMode(mot_fwd_pin[1], OUTPUT);
    pinMode(mot_rev_pin[0], OUTPUT);
    pinMode(mot_rev_pin[1], OUTPUT);
    pinMode(mot_spd_pin[0], OUTPUT);
    pinMode(mot_spd_pin[1], OUTPUT);

#if !ROMIBO_AUDIO_PWM_ENABLED
    pinMode(irPin, INPUT); // conflict with pwm output
#endif

#endif

#if (ROMIBO_ELECTRONICS_MAJOR_REV==3)
    pinMode( mot_drv1_pin[0], OUTPUT );
    pinMode( mot_drv1_pin[1], OUTPUT );
    pinMode( mot_drv2_pin[0], OUTPUT );
    pinMode( mot_drv2_pin[1], OUTPUT );
    pinMode( motor_sleep_pin, OUTPUT );	// *** Caz *** so I'm just going to see if this works for now
    // but we might wanna write some functions to put the motor drive IC 
    // in sleep or wake mode as we see fit.
    digitalWrite( motor_sleep_pin, HIGH );// Enable the motor driver

    // Setup the shutdown pin on the audio amplifier	// *** Caz ***
    pinMode(amp_shutdownPin, OUTPUT);

    // Turn on the audio amplifier
    digitalWrite(amp_shutdownPin, HIGH);// *** Caz ***

    pinMode(irPin, INPUT);
#endif
   
    // Set up accelerometers
    accel_mobo = new Accelerometer();
    (accel_mobo->acc) = new MMA8453_n0m1();
    (accel_mobo->acc)->setI2CAddr(accel_add_mobo);
    (accel_mobo->acc)->dataMode(true,2);
    (accel_mobo->acc)->regWrite(0x1,0);
    (accel_mobo->acc)->regWrite(0x3,0);
    (accel_mobo->acc)->regWrite(0x5,0);
    (accel_mobo->vec) = new Vector3();

    accel_hbbo = new Accelerometer();
    (accel_hbbo->acc) = new MMA8453_n0m1();
    (accel_hbbo->acc)->setI2CAddr(accel_add_hbbo);
    (accel_hbbo->acc)->dataMode(true,2);
    (accel_hbbo->acc)->regWrite(0x1,0);
    (accel_hbbo->acc)->regWrite(0x3,0);
    (accel_hbbo->acc)->regWrite(0x5,0);
    (accel_hbbo->vec) = new Vector3();

    pinMode(rgb_led[0], OUTPUT);
    pinMode(rgb_led[1], OUTPUT);
    pinMode(rgb_led[2], OUTPUT);
    pinMode(dac_csPin, OUTPUT);
    pinMode(dac_sckPin, OUTPUT);
    pinMode(dac_sdiPin, OUTPUT);

#if (ROMIBO_ELECTRONICS_MAJOR_REV==1) || (ROMIBO_ELECTRONICS_MAJOR_REV==2)
    pinMode(dac_ldacPin, OUTPUT);
#endif

#if ROMIBO_AUDIO_PWM_ENABLED
    pinMode( audio_PWM_pin, OUTPUT );  // used as OC1C during playback, otherwise normal output
    digitalWrite( audio_PWM_pin, LOW );
#endif

    tilt_servo[0].attach(tilt_servo_pin[0]);
    tilt_servo[1].attach(tilt_servo_pin[1]);
    tilt_servo[2].attach(tilt_servo_pin[2]);

    // Set the motors, servos, and LEDs to something safe.
    stop();
    setEyelid( 50 );
    setNeutralHeadPosition();
    setAntennaColor( 0, 100, 0 ); // dim green

    // Randomizes the random number generator by reading a floating pin's value
    randomSeed(analogRead(unassigned_pin));

    // Setup the SD card for sound
    int err = sdSetup();
    if (err) {
        Serial.print("SD ERR: ");
        Serial.println(err);
    }

    Serial.println("RomiboRobot initialized.");
}
/****************************************************************/
unsigned RomiboRobot::sensorSamples(void)   { return cycle_count; }

int RomiboRobot::frontRangeDistance(void)   { poll(); return range.scaled; }
int RomiboRobot::topLightLevel(void)        { poll(); return photo_top.scaled; }
int RomiboRobot::leftFrontLightLevel(void)  { poll(); return photo_left.scaled; }
int RomiboRobot::rightFrontLightLevel(void) { poll(); return photo_right.scaled; }
int RomiboRobot::soundLevel(void)           { poll(); return microphone.scaled; }
int RomiboRobot::readAccMobo(char axis)     { poll(); 
                                                switch(axis)
                                                {
                                                    case 'x':
                                                        return (accel_mobo->vec)->x;
                                                        break;
                                                    case 'y':
                                                        return (accel_mobo->vec)->y;
                                                        break;
                                                    case 'z':
                                                        return (accel_mobo->vec)->z;
                                                        break;
                                                }
                                                    return -1;
                                                }
int RomiboRobot::readAccHbbo(char axis)     { poll(); 
                                                switch(axis)
                                                {
                                                    case 'x':
                                                        return (accel_hbbo->vec)->x;
                                                        break;
                                                    case 'y':
                                                        return (accel_hbbo->vec)->y;
                                                        break;
                                                    case 'z':
                                                        return (accel_hbbo->vec)->z;
                                                        break;
                                                }
                                                    return -1;
                                            }


int RomiboRobot::obstacleProximity(void)    { poll();  return discretized_sensor_value( &range ); }
int RomiboRobot::topBrightness(void)        { poll();  return discretized_sensor_value( &photo_top ); }
int RomiboRobot::leftBrightness(void)       { poll();  return discretized_sensor_value( &photo_left ); }
int RomiboRobot::rightBrightness(void)      { poll();  return discretized_sensor_value( &photo_right ); }

int RomiboRobot::frontLightHeading(void) { return 0; }

// FIXME: the switch inputs should be properly debounced from the polling cycle.
// But be careful, this might be called before polling begins.

// Return true if the given switch is pressed.
int RomiboRobot::touchSwitch(int switchnum) 
{
    // The switches are normally-open SPST with a pullup, so a low value represents contact.
    return (LOW == digitalRead( touch_sensor[ constrain(switchnum, 0, ROMIBO_NUM_TOUCH_SWITCHES-1) ] ));
}

int RomiboRobot::topSwitch(void) { return touchSwitch(ROMIBO_TOUCH_TOP); }
int RomiboRobot::leftSwitch(void) { return touchSwitch(ROMIBO_TOUCH_LEFT); }
int RomiboRobot::rightSwitch(void) { return touchSwitch(ROMIBO_TOUCH_RIGHT); }
int RomiboRobot::bottomSwitch(void) { return touchSwitch(ROMIBO_TOUCH_BOTTOM); }

unsigned RomiboRobot::touchSwitches(void) 
{
    // Construct a word with one bit per switch in the same order as the pin definitions.
    // The switches are normally-open SPST with a pullup, so a low value represents contact.
    // The first touch switch is the least significant bit.
    unsigned value = 0, mask = 1;
    for (int i = 0; i < ROMIBO_NUM_TOUCH_SWITCHES; i++ ) {
        if (digitalRead( touch_sensor[i] ) == LOW ) value |= mask;
        mask <<= 1;
    }
    return value;
}

int RomiboRobot::getRemoteControlKey(void) { return 0; }


/****************************************************************/
#if (ROMIBO_ELECTRONICS_MAJOR_REV==1) || (ROMIBO_ELECTRONICS_MAJOR_REV==2)
// A shim to ease the translation of older code.  This should be phased out.
// The motor index is either ROMIBO_LEFT_MOTOR or ROMIBO_RIGHT_MOTOR.
// dir is positive for forward, else reverse
// spd is a positive speed value on [0, 255]
void RomiboRobot::setDriveMotor( int mot, int dir, int spd )
{
    if (dir > 0) {
        digitalWrite(mot_fwd_pin[mot], HIGH); // configure H-bridge for forward current
        digitalWrite(mot_rev_pin[mot], LOW);
    }

    else {
        digitalWrite(mot_fwd_pin[mot], LOW);  // configure H-bridge for reverse current
        digitalWrite(mot_rev_pin[mot], HIGH);
    }

    // Set the motor PWM output.  Note that zero speed will drive the enable line
    // low, which will allow the drivers to float.  For this reason the undriven
    // state is free wheeling instead of braking.
    analogWrite(mot_spd_pin[mot], spd);     

    // record the last output value
    int calibrated_speed = map( spd, 0, 255, 0, 100 );
    if ( dir <= 0 )  calibrated_speed = -calibrated_speed;
    if ( mot == ROMIBO_LEFT_MOTOR ) left_speed = calibrated_speed;
    else                            right_speed = calibrated_speed;
}

void RomiboRobot::drive(int leftSpeed, int rightSpeed)
{
    leftSpeed  = constrain(leftSpeed,  -ROMIBO_DRIVE_MOTOR_LIMIT, ROMIBO_DRIVE_MOTOR_LIMIT );
    rightSpeed = constrain(rightSpeed, -ROMIBO_DRIVE_MOTOR_LIMIT, ROMIBO_DRIVE_MOTOR_LIMIT );
    
    setDriveMotor( ROMIBO_LEFT_MOTOR,  leftSpeed,  constrain( map(abs(leftSpeed ), 0, 100, 0, 255), 0, 255 ) );;
    setDriveMotor( ROMIBO_RIGHT_MOTOR, rightSpeed, constrain( map(abs(rightSpeed), 0, 100, 0, 255), 0, 255 ) );
}
#endif

/****************************************************************/
/****************************************************************/
// The Rev3 electronics uses a TI DRV8833 dual H-bridge driver, which has a
// different set of control signals.

// Generic way of setting speed on a motor.
// speed should be in range of [-100,100]
// motor should be either "ROMIBO_LEFT_MOTOR" or "ROMIBO_RIGHT_MOTOR"
void setMotor(int speed, int motor)
{

  //adjustment for new wiring (5/27) that caused robot to run backwards
  speed = -speed;

    #define DRIVEPWMRATE 20000L
    #define DRIVEPWMDIVISOR F_CPU/(2*DRIVEPWMRATE)

    // Bound the speed to be within the upper and lower limits
    speed = constrain(speed, -ROMIBO_DRIVE_MOTOR_LIMIT, ROMIBO_DRIVE_MOTOR_LIMIT);

    // We do not have to adjust to how the motors are inverted on the robot
    // Either it is already adjusted in the pin configuration
    // Or it is adjusted in the iOS app.

    if(speed == 0) {
        // (PIN1,PIN2) = (0,0) means the motor is off
        digitalWrite(mot_drv1_pin[motor],LOW);
        digitalWrite(mot_drv2_pin[motor],LOW);
    }
    else if(speed > 0) {
        // (PIN1,PIN2) = (+x,0)
        // Motor is turning forwards
        // NOTE: Forwards does not mean moving forward due to the motor inversion
        if(speed >= 100) {
            // (PIN1,PIN2) = (1,0)
            // Motor is max speed forwards
            digitalWrite(mot_drv1_pin[motor],HIGH);
            digitalWrite(mot_drv2_pin[motor],LOW);
        }
        else {
            // (PIN1,PIN2) = (+x,0), 0 < x < 1
            // Motor is turning forwards, at some non-max speed
            // Scale the speed from 0 to 100.
            uint16_t pwm = map( abs(speed), 0, 100, 0, DRIVEPWMDIVISOR);
            digitalWrite(mot_drv2_pin[motor],LOW);
            analogWrite (mot_drv1_pin[motor], pwm);
            // NOTE: Possible solution is to segment the speed in chunks,
            // as in 0-20, 21-40, 41-60, 61-80, 81-100
            // And then the speed differences might be more apparent
            // via analogWrite.
            //
            // Or perhaps the PWM doesn't work correctly within analogWrite
        }
    }
    else {
        // (PIN1,PIN2) = (0,-x) means the motor is turning backwards
        // NOTE: Forwards does not mean moving forward due to the motor inversion
        if(speed <= -100) {
            digitalWrite(mot_drv1_pin[motor],LOW);
            digitalWrite(mot_drv2_pin[motor],HIGH);
        }
        else {
            uint16_t pwm = map( abs(speed), 0, 100, 0, DRIVEPWMDIVISOR);
            digitalWrite(mot_drv1_pin[motor],LOW);
            analogWrite (mot_drv2_pin[motor], pwm);
        }
    }
}

#if (ROMIBO_ELECTRONICS_MAJOR_REV==3)

void RomiboRobot::drive(int leftSpeed, int rightSpeed)
{
    // For fast-decay current-limiting on the DRV8833, the following signals are required:
    //   Forward     IN1=PWM  IN2=0
    //   Reverse     IN1=0    IN2=PWM
    //   Coast       IN1=0    IN2=0

    // For both motors, the timer channel are used in 16-bit phase correct PWM mode
    // The left motor is on Timer3, the right motor is on Timer4:
    //   The overall PWM rate is set by OCRnA.
    //   DRV1 is controlled by OCnB
    //   DRV2 is controlled by OCnC

    // The motor outputs use 16-bit PWM:
    #define DRIVEPWMRATE 20000L
    #define DRIVEPWMDIVISOR F_CPU/(2*DRIVEPWMRATE)

    // Sets the appropriate speed to the appropriate motor
    setMotor(leftSpeed,ROMIBO_LEFT_MOTOR);
    setMotor(rightSpeed,ROMIBO_RIGHT_MOTOR);
}
#undef DRIVEPWMRATE
#undef DRIVEPWMDIVISOR

#endif

/****************************************************************/
/****************************************************************/

void RomiboRobot::stop(void)               { drive( 0, 0 );  }
void RomiboRobot::driveForward(int speed)  { drive(  speed,  speed ); }
void RomiboRobot::driveBackward(int speed) { drive( -speed, -speed ); }
void RomiboRobot::spinInPlace(int speed)   { drive( -speed,  speed ); }

void RomiboRobot::driveTowardLight(int speed) { }

int RomiboRobot::leftSpeed(void)  { return left_speed;  }
int RomiboRobot::rightSpeed(void) { return right_speed; }

/****************************************************************/
void RomiboRobot::blink(void)
{
  setEyelid(10);
  delay(200);
  setEyelid(100); 
}


void RomiboRobot::setEyelid(int eye_open_position) 
{
    tilt_servo[ ROMIBO_EYE_SERVO ].write( map( constrain(eye_open_position, 0, 100), 0, 100, eye_max, eye_min ) );  
    eye_pos = eye_open_position;
}

void RomiboRobot::setHeadPosition( int front_position, int back_position ) 
{
  //tilt_servo[ ROMIBO_FRONT_SERVO ].write( front_position );
  //tilt_servo[ ROMIBO_BACK_SERVO  ].write( back_position );
   
  int yAxis = map( constrain(front_position,0,100), 0, 100, front_min, front_max);
  int xAxis = map( constrain(back_position,0,100), 0, 100, back_min, back_max);

    tilt_servo[ ROMIBO_FRONT_SERVO ].write( yAxis );
    tilt_servo[ ROMIBO_BACK_SERVO  ].write( xAxis );

    front_pos = front_position;
    back_pos  = back_position;
}
void RomiboRobot::setHeadPosition( int neck_positions[2] ) 
{
    setHeadPosition( neck_positions[0], neck_positions[1] );
}


void RomiboRobot::setNeutralHeadPosition(void) 
{
    setHeadPosition( 50, 50 );
}

void RomiboRobot::tiltHeadForward(void)
{
    setHeadPosition( 0, 100 );
}

void RomiboRobot::tiltHeadBack(void) 
{
    setHeadPosition( 100, 0 );
}

void RomiboRobot::bob(void)
{
  setHeadPosition (0,0);
  delay (1000);
  setHeadPosition (100,100);
  delay (1000);
  setNeutralHeadPosition ();
}



/****************************************************************/
void RomiboRobot::setAntennaColor( uint8_t red, uint8_t green, uint8_t blue ) 
{
    analogWrite(rgb_led[1], green);
    analogWrite(rgb_led[2], blue );

#if ROMIBO_ELECTRONICS_MAJOR_REV<3
    // In Rev1 and Rev2, red was tied to OC1A, but Timer1 is used for audio, so hardware PWM isn't available.
    digitalWrite(rgb_led[0], (red  ) ? HIGH : LOW );
#else
    // In Rev3, red is tied to OC0A, which can be driven by Timer0.
    analogWrite(rgb_led[0], red  );
#endif
}

void RomiboRobot::setAntennaColor( uint8_t color[3] ) 
{
    setAntennaColor( color[0], color[1], color[2] );
}

void RomiboRobot::setAntennaColor( int color[3] )
{
    setAntennaColor( color[0], color[1], color[2] );
}

void RomiboRobot::setAntennaColorWhite(void)
{ 
  setAntennaColor(255, 255, 255); // White
}

void RomiboRobot::setAntennaColorGreen(void)
{ 
  setAntennaColor(0, 255, 0); // White
}



/****************************************************************/
void RomiboRobot::delayMicroseconds(unsigned long microseconds_duration) 
{
    ElapsedTimer waitTimer;
    do {
        poll();
    } while( waitTimer.elapsedMicroseconds() < microseconds_duration);
}

void RomiboRobot::delay(unsigned long milliseconds_duration) 
{
    delayMicroseconds( 1000 * milliseconds_duration );
}

void RomiboRobot::waitForNewSensorData(void) 
{
    unsigned current_cycle = cycle_count;
    do {
        // wait for cycle_count to update
        poll();
    } while (current_cycle == cycle_count);
}

/****************************************************************/

void RomiboRobot::playWavFile(char* wavFileName)
{
  playfile(wavFileName);
}


void RomiboRobot::playSoundNamed(const char *basename) 
{
    char name[13];          // space for a DOS 8.3 filename
    strncpy( name, basename, 8);  // copy up to 8 characters from the arglist
    name[8] = 0;            // make sure it is null-terminated
    strcat( name, ".WAV" ); // add the file suffix
    playfile(name);
}

int RomiboRobot::isSoundPlaying(void) 
{
    return soundIsPlaying();
}

void RomiboRobot::stopSound(void) 
{
    stopSound();
}
void RomiboRobot::waitForSoundToFinish(void)
{
    // Keep processing I/O while the sound plays.  This should also incorporate a
    // timeout in case something goes wrong.
    while( isSoundPlaying() ) poll();
}

bool RomiboRobot::soundIsEnabled(void) 
{
    return soundEnabled(); 
}

/****************************************************************/
// The following would eventually make sense to implement, though perhaps not right away:
#if 0
void RomiboRobot::bounceHeadUpAndDown(int speed, int cycles) { }
void RomiboRobot::turnInPlaceForAngle( int heading ) { }
void RomiboRobot::whenSoundEnds( handler_function_t handler) { }
void RomiboRobot::whenTouched( handler_function_t handler ) { }
void RomiboRobot::whenIdle( handler_function_t handler ) { }
void RomiboRobot::setPeriodicTimer( handler_function_t handler_function_t, int milliseconds_interval ) { }
#endif


/****************************************************************/
// Declare the singleton Romibo object.  This follows the general Arduino
// convention of providing a global initialized object so that user code doesn't
// need to.  The downside of this convention is that it makes it more difficult
// for a user to subclass the object and provide an augmented implementation.

// Global object for accessing the robot hardware.
RomiboRobot Romibo;   

/****************************************************************/
// Provide a descriptor table which is added to the global parameter
// set to allow interactive debugging at runtime.

struct parameter_descriptor_t RomiboRobot::parameter_descriptor_table[] = {
    { "overruns",             &Romibo.overruns },
    { "photo_left.average",   &Romibo.photo_left.average },
    { "photo_left.max",       &Romibo.photo_left.max },
    { "photo_left.min",       &Romibo.photo_left.min },
    { "photo_left.raw",       &Romibo.photo_left.raw },
    { "photo_left.scaled",    &Romibo.photo_left.scaled },
    { "photo_left.smoothed",  &Romibo.photo_left.smoothed },
    { "photo_left.thresh1",   &Romibo.photo_left.thresh1 },
    { "photo_left.thresh2",   &Romibo.photo_left.thresh2 },
    { "photo_right.average",  &Romibo.photo_right.average },
    { "photo_right.max",      &Romibo.photo_right.max },
    { "photo_right.min",      &Romibo.photo_right.min },
    { "photo_right.raw",      &Romibo.photo_right.raw },
    { "photo_right.scaled",   &Romibo.photo_right.scaled },
    { "photo_right.smoothed", &Romibo.photo_right.smoothed },
    { "photo_right.thresh1",  &Romibo.photo_right.thresh1 },
    { "photo_right.thresh2",  &Romibo.photo_right.thresh2 },
    { "photo_top.average",    &Romibo.photo_top.average },
    { "photo_top.max",        &Romibo.photo_top.max },
    { "photo_top.min",        &Romibo.photo_top.min },
    { "photo_top.raw",        &Romibo.photo_top.raw },
    { "photo_top.scaled",     &Romibo.photo_top.scaled },
    { "photo_top.smoothed",   &Romibo.photo_top.smoothed },
    { "photo_top.thresh1",    &Romibo.photo_top.thresh1 },
    { "photo_top.thresh2",    &Romibo.photo_top.thresh2 },
    { "range.average",        &Romibo.range.average },
    { "range.max",            &Romibo.range.max },
    { "range.min",            &Romibo.range.min },
    { "range.raw",            &Romibo.range.raw },
    { "range.scaled",         &Romibo.range.scaled },
    { "range.smoothed",       &Romibo.range.smoothed },
    { "range.thresh1",        &Romibo.range.thresh1 },
    { "range.thresh2",        &Romibo.range.thresh2 },
    { "microphone.average",   &Romibo.microphone.average },
    { "microphone.max",       &Romibo.microphone.max },
    { "microphone.min",       &Romibo.microphone.min },
    { "microphone.raw",       &Romibo.microphone.raw },
    { "microphone.scaled",    &Romibo.microphone.scaled },
    { "microphone.smoothed",  &Romibo.microphone.smoothed },
    { "microphone.thresh1",   &Romibo.microphone.thresh1 },
    { "microphone.thresh2",   &Romibo.microphone.thresh2 },

    { "front.pos",  	    &Romibo.front_pos },
    { "front.min",  	    &Romibo.front_min },
    { "front.max",  	    &Romibo.front_max },

    { "back.pos ",  	    &Romibo.back_pos  },
    { "back.min ",  	    &Romibo.back_min  },
    { "back.max ",  	    &Romibo.back_max  },

    { "eye.pos ",   	    &Romibo.eye_pos  },
    { "eye.min ",   	    &Romibo.eye_min  },
    { "eye.max ",   	    &Romibo.eye_max  },

    { "left.speed",           &Romibo.left_speed },
    { "right.speed",          &Romibo.right_speed },

};

struct parameter_descriptor_t *RomiboRobot::parameters_table(void) 
{
    return (RomiboRobot::parameter_descriptor_table); 
}

size_t RomiboRobot::parameters_table_size(void) 
{ 
    return sizeof(RomiboRobot::parameter_descriptor_table); 
}

/****************************************************************/

/****************************************************************/
