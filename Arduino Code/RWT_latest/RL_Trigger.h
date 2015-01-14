/*
  RL_Trigger.h - Library for triggering a BT TM7 paintball marker.
  Version 1.1
  Created by Bob Rudolph, April 28, 2014.
  Rudolph Labs:  http://www.rudolphlabs.com/
*/

// ensure this library description is only included once
#ifndef RL_Trigger_h
#define RL_Trigger_h

#include "Arduino.h"

// library interface description
class Trigger {

  public:
    // constructors:
    Trigger(int trigger_pin);

    // methods to override default values:
    void set_delay(int delay_milliseconds);
    void set_dwell(int dwell_milliseconds);

    // methods to fire gun:
    void squeeze();
    void release();
    void single_shot();
    void burst(int num_shots);

    boolean is_squeezed();

  private:
    // pin numbers:
    int _trigger_pin;

    // delay parameters:
    int _delay;
    int _dwell;

    boolean _is_squeezed;

};

#endif
