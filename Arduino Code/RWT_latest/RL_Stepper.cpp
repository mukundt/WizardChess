/*
  RL_Stepper.cpp - Library for controlling stepper motors.
  Version 1.0
  Created by Bob Rudolph, February 13, 2014.
  Rudolph Labs:  http://www.rudolphlabs.com/
*/

#include "RL_Stepper.h"

// constructor:
Stepper::Stepper(int step_pin, int direction_pin, int enable_pin)
{
  // Initialize parameters to default values:
  _delay = 2000;
  _delay_in_microseconds = true;
  _dwell_microseconds = 100;
  _invert_direction = false;
  _count = 0;
  _is_enabled = false;

  // Save pin numbers:
  _step_pin = step_pin;
  _enable_pin = enable_pin;
  _direction_pin = direction_pin;

  // Setup these pins as outputs:
  pinMode(_step_pin, OUTPUT);
  pinMode(_enable_pin, OUTPUT);
  pinMode(_direction_pin, OUTPUT);

  // Disable the motor to start with:
  disable();
}

// Sets the delay time (in milliseconds) between successive step commands.
void Stepper::set_delay(float delay_milliseconds)
{
  if(delay_milliseconds < 5.0) {
    // Arduino's delay() function is not too accurate for low values, so for
    // small delay times we choose to use delayMicroseconds() instead.
    _delay = int(delay_milliseconds * 1000);
    _delay_in_microseconds = true;
  }else {
    // With a higher value, we are ok using delay().
    _delay = int(delay_milliseconds);
    _delay_in_microseconds = false;
  }
}

// Sets the dwell time (in microseconds) that the step pin will remain high for.
void Stepper::set_dwell(int dwell_microseconds)
{
  _dwell_microseconds = dwell_microseconds;
}

// Overrides the current step count, sets it to new_count.
void Stepper::set_count(int new_count)
{
  _count = new_count;
}

// Inverts the direction of the stepper motor.
void Stepper::invert_direction(boolean yes_no)
{
  _invert_direction = yes_no;
}

// Moves the motor the specified number of steps. A negative number of steps will
// move the motor in the opposite direction. This function blocks.
// If the motor was previously off, this function will turn it on.
void Stepper::move_relative(int num_steps)
{
  _count += num_steps; // update the step counter
  enable();
  boolean direction = (num_steps >= 0);  // if num_steps is negative, go backwards
  if(_invert_direction) {
    direction = !direction;  // invert if needed
  }
  digitalWrite(_direction_pin, direction);
  num_steps = abs(num_steps);
  for(int step_number = 0; step_number < num_steps; step_number++) {
    digitalWrite(_step_pin, HIGH);
    delayMicroseconds(_dwell_microseconds);
    digitalWrite(_step_pin, LOW);
    if(_delay_in_microseconds) {
      delayMicroseconds(_delay - _dwell_microseconds);
    }else {
      delay(_delay - int(_dwell_microseconds / 1000));
    }
  }
}

// Moves the motor to an absolute step position. This function blocks.
// If the motor was previously off, this function will turn it on.
void Stepper::move_absolute(int step_number)
{
  move_relative(step_number - _count);
  _count = step_number;
}

// SPECIAL USE
// This function sends a pulse then exits, without waiting the proper delay time for the motor to move a step.
// Caution must be exercised to take care of the delay externally to this function. When properly used, this
// function will allow you to move multiple motors synchronously without slowing down individual motors.
// tl;dr: only use this function if you know what you're doing
// dir = -1 to move backwards, 1 to move forwards (zero does nothing, all other values cropped to [-1, 1] )
void Stepper::single_step(int dir)
{
  if (dir > 1) dir = 1;
  if (dir < -1) dir = -1;
  _count += dir; // update the step counter
  enable();
  boolean direction = (dir >= 0);  // if num_steps is negative, go backwards
  if(_invert_direction) {
    direction = !direction;  // invert if needed
  }
  digitalWrite(_direction_pin, direction);
  digitalWrite(_step_pin, HIGH);
  delayMicroseconds(_dwell_microseconds);
  digitalWrite(_step_pin, LOW);
}

// SPECIAL USE
// This function simply delays for the specified delay time of the motor.
// This is meant to used in conjuction with single_step(), to allow you to move multiple motors 
// synchronously without slowing down individual motors.
// tl;dr: only use this function if you know what you're doing
void Stepper::execute_delay()
{
  if(_delay_in_microseconds) {
    delayMicroseconds(_delay - _dwell_microseconds);
  }else {
    delay(_delay - int(_dwell_microseconds / 1000));
  }
}

// Disables the motor, releasing torque on the output shaft.
// CAUTION: disabling a motor allows the shaft to spin freely, which could mess up step counts.
void Stepper::disable()
{
  digitalWrite(_enable_pin, HIGH); // this pin has inverse logic
  _is_enabled = false;
}

// Enables the motor, which puts full holding torque on the output shaft.
// CAUTION: leaving a motor on for too long can cause it to overheat.
void Stepper::enable()
{
  digitalWrite(_enable_pin, LOW); // this pin has inverse logic
  _is_enabled = true;
}

// Returns the current step count.
int Stepper::get_count()
{
  return _count;
}

// Returns the enabled state of the motor (true => is enabled).
boolean Stepper::is_enabled()
{
  return _is_enabled;
}
