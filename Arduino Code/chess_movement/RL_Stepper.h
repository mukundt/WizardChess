/*
  RL_Stepper.h - Library for controlling stepper motors.
  Version 1.0
  Created by Bob Rudolph, February 13, 2014.
  Rudolph Labs:  http://www.rudolphlabs.com/
*/

// ensure this library description is only included once
#ifndef RL_Stepper_h
#define RL_Stepper_h

#include "Arduino.h"

// library interface description
class Stepper {

  public:
    // constructors:
    Stepper(int step_pin, int direction_pin, int enable_pin);

    // methods to override default values:
    void set_delay(float delay_milliseconds);
    void set_dwell(int dwell_microseconds);
    void set_count(int new_count);
    void invert_direction(boolean yes_no);

    // methods to move motor:
    void move_relative(int num_steps);
    void move_absolute(int step_number);
    void single_step(int direction);
    void execute_delay();

    // methods to enable/disable motor:
    void disable();
    void enable();

    int get_count();
    boolean is_enabled();

  private:
    // pin numbers:
    int _step_pin;
    int _enable_pin;
    int _direction_pin;

    // delay parameters:
    int _delay;
    boolean _delay_in_microseconds;
    int _dwell_microseconds;

    boolean _invert_direction;
    int _count;
    boolean _is_enabled;

};

#endif
