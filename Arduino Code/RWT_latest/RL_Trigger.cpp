/*
  RL_Trigger.cpp - Library for triggering a BT TM7 paintball marker.
  Version 1.0
  Created by Bob Rudolph, February 24, 2014.
  Rudolph Labs:  http://www.rudolphlabs.com/
*/

#include "RL_Trigger.h"

// constructor:
Trigger::Trigger(int trigger_pin)
{
  // Initialize parameters to default values:
  _delay = 50;
  _dwell = 50;
  _is_squeezed = false;

  // Save pin number:
  _trigger_pin = trigger_pin;

  // Setup pin as output:
  pinMode(_trigger_pin, OUTPUT);

  // Release the trigger to start with:
  release();
}

// Sets the delay time (in milliseconds) between successive shots.
void Trigger::set_delay(int delay_milliseconds)
{
  _delay = delay_milliseconds;
}

// Sets the dwell time (in microseconds) to hold the trigger for one shot.
void Trigger::set_dwell(int dwell_milliseconds)
{
  _dwell = dwell_milliseconds;
}

// Squeezes the trigger and holds it indefinitely.
void Trigger::squeeze()
{
  digitalWrite(_trigger_pin, LOW);
  _is_squeezed = true;
}

// Releases the trigger.
void Trigger::release()
{
  digitalWrite(_trigger_pin, HIGH);
  _is_squeezed = false;
}

// Fires a single shot. Built-in delay after shot to allow next ball to chamber.
void Trigger::single_shot()
{
  squeeze();
  delay(_dwell);
  release();
  delay(_delay);
}

// Fires the specified number of shots in rapid succession.
void Trigger::burst(int num_shots)
{
  for (int i = 0; i < num_shots; i++) {
    single_shot();
  }
}

// Returns the state of the trigger (True == squeezed).
boolean Trigger::is_squeezed()
{
  return _is_squeezed;
}