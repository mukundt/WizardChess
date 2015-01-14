#define EYES_OFF true

#define SERIAL_TIMEOUT_MILLIS 300

#define FIRING_RATE_DELAY_MILLIS 200

#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include <RBL_services.h>
#include "RL_Stepper.h"
#include "RL_Trigger.h"

#define pan_step_pin 14
#define pan_direction_pin 15
#define pan_enable_pin 16

#define tilt_step_pin 17
#define tilt_direction_pin 18
#define tilt_enable_pin 19

#define trigger_pin 6   // purple
#define PIN_POWER 3    // red & yellow
#define PIN_SAFETY_A 5 // blue
#define PIN_SAFETY_B 4 // green
#define PIN_HOPPER 7

#define PIN_POWER_DETECT 2

#define PAN_MAX_POSITION 2500   // right extreme
#define PAN_MIN_POSITION -2500  // left extreme
#define TILT_MAX_POSITION 2000  // down extreme
#define TILT_MIN_POSITION -1000 // up extreme

Stepper pan(pan_step_pin, pan_direction_pin, pan_enable_pin);
Stepper tilt(tilt_step_pin, tilt_direction_pin, tilt_enable_pin);
Trigger trigger(trigger_pin);

boolean pan_left, pan_right;
boolean tilt_up, tilt_down;
boolean scanning;
boolean scan_direction;  // true = right, false = left
boolean trigger_on;
boolean high_rate_of_fire = false;

boolean gun_armed = false;
boolean power_on = false;
boolean power_on_prev = false;

unsigned long last_packet_timestamp = 0;
unsigned long last_shot_timestamp = 0;
unsigned long start_squeeze_timestamp = 0;

void setup() {
  pinMode(PIN_POWER, OUTPUT);
  pinMode(PIN_SAFETY_A, OUTPUT);
  pinMode(PIN_SAFETY_B, OUTPUT);
  pinMode(PIN_HOPPER, OUTPUT);
  pinMode(PIN_POWER_DETECT, INPUT);
  
  digitalWrite(PIN_POWER, LOW);
  power_on_prev = digitalRead(PIN_POWER_DETECT);
  power_on = digitalRead(PIN_POWER_DETECT);
  
  safety_on();
  
  pan.enable();
  tilt.enable();
 
  pan_left = pan_right = tilt_up = tilt_down = trigger_on = scanning = false; 

  ble_begin();
  
  Serial.begin(57600);
  
//  disarm_gun();
//  delay(2000);
//  arm_gun();
}

void loop() {
  while (Serial.available() > 0)   {
    last_packet_timestamp = millis();
    char command = (char)Serial.read();
    char velocity = '#';
    if (command == 'q' || command == 'w' || command == 'e' || command == 'r' || command == 't' || command == 'y' || command == 'u' || command == 'i') {
      while (velocity != '0' && velocity != '1' && velocity != '2' && velocity != '3' && velocity != '4' && velocity != '5') {
        velocity = (char)Serial.read(); 
      }
    }
    switch (velocity)
    {
      case '0':
        pan.set_delay(8.0);
        tilt.set_delay(8.0);
        break;
      case '1':
        pan.set_delay(5.0);
        tilt.set_delay(5.0);
        break;
      case '2':
        pan.set_delay(3.0);
        tilt.set_delay(3.0);
        break;
      case '3':
        pan.set_delay(2.0);
        tilt.set_delay(2.0);
        break;
      case '4':
        pan.set_delay(1.0);
        tilt.set_delay(1.0);
        break;
      case '5':
        pan.set_delay(0.5);
        tilt.set_delay(0.5);
        break;
      default: 
        break;
    }
    switch (command)
    {
      case 'q':
        pan_right = true;
        pan_left = false;
        tilt_up = false;
        tilt_down = false;
        scanning = false;
        break;
      case 'w':
        pan_right = true;
        pan_left = false;
        tilt_up = true;
        tilt_down = false;
        scanning = false;
        break;
      case 'e':
        pan_right = false;
        pan_left = false;
        tilt_up = true;
        tilt_down = false;
        scanning = false;
        break;
      case 'r':
        pan_right = false;
        pan_left = true;
        tilt_up = true;
        tilt_down = false;
        scanning = false;
        break;
      case 't':
        pan_right = false;
        pan_left = true;
        tilt_up = false;
        tilt_down = false;
        scanning = false;
        break;
      case 'y':
        pan_right = false;
        pan_left = true;
        tilt_up = false;
        tilt_down = true;
        scanning = false;
        break;
      case 'u':
        pan_right = false;
        pan_left = false;
        tilt_up = false;
        tilt_down = true;
        scanning = false;
        break;
      case 'i':
        pan_right = true;
        pan_left = false;
        tilt_up = false;
        tilt_down = true;
        scanning = false;
        break;
      case 'p': 
        pan_left = false;
        pan_right = false;
        tilt_up = false;
        tilt_down = false;
        trigger_on = false;
        scanning = false;
        break;
      case 'f':
        trigger_on = true;
        break;
      case 'n':
        trigger_on = false;
        break;
      case 'k':
        arm_gun();
        break;
      case 'l':
        disarm_gun();
        break;
      case '+':
        high_rate_of_fire = true;
        break;
      case '-':
        high_rate_of_fire = false;
        break;
      case 'z':
        zero();
        break;
      case 's':
        scanning = true;
      default: 
        break;
    }
  }
  
  while (ble_available())
  {
    char command = (char)ble_read();
    switch (command)
    {
      case 'k':
          Serial.println("received k");
        break;
      case 'l':
          Serial.println("received l"); 
        break;
      default: 
        break;
    }
  }
  
  power_on_prev = power_on;
  power_on = digitalRead(PIN_POWER_DETECT);
  
  if (power_on && !power_on_prev) {
    arm_gun();
    zero();
  } else if (!power_on && power_on_prev) {
    disarm_gun();
  }
    
  
  if (millis() - last_packet_timestamp > SERIAL_TIMEOUT_MILLIS) {
    pan_left = false;
    pan_right = false;
    tilt_up = false;
    tilt_down = false;
    trigger_on = false;
    scanning = false;
  }
  
  if (scanning) {
    tilt.set_delay(0.5);
    tilt.move_absolute(0);
    pan.set_delay(0.5);
    pan_left = false;
    pan_right = false;
    tilt_up = false;
    tilt_down = false;
    trigger_on = false;
    if (scan_direction) { // go right
      if (pan.get_count() >= PAN_MAX_POSITION) {
        scan_direction = false; // switch to left
      }else{
        pan_right = true;
        if (pan.get_count() >= PAN_MAX_POSITION - 100) {
          pan.set_delay(1.2);
        }else{
          pan.set_delay(0.5);
        }
      } 
    }else{               // go left
      if (pan.get_count() <= PAN_MIN_POSITION) {
        scan_direction = true; // switch to right
      }else{
        pan_left = true;
        if (pan.get_count() <= PAN_MIN_POSITION + 100) {
          pan.set_delay(1.2);
        }else{
          pan.set_delay(0.5);
        }
      } 
    }
  }
  
  //sends relevant command every clock cycle
  if (pan_left) pan.single_step(-1);
  if (pan_right) pan.single_step(1);
  if (tilt_up) tilt.single_step(-1);
  if (tilt_down) tilt.single_step(1);
  pan.execute_delay();

  if (trigger_on && power_on) {
    if (millis() - start_squeeze_timestamp < 50) {
      trigger.squeeze();
    }else if ( (!high_rate_of_fire && millis() - start_squeeze_timestamp < 100 + FIRING_RATE_DELAY_MILLIS) ||
               (high_rate_of_fire && millis() - start_squeeze_timestamp < 70) ) {
      trigger.release();
    }else{
      trigger_on = false;
    }
  }else{
    start_squeeze_timestamp = millis();
  }
  
  ble_do_events();

}

void arm_gun() {
  delay(50);
  Serial.println("enabled");
  safety_on();
  if (EYES_OFF) {
    trigger.squeeze();
  }
  gun_on();
  if (EYES_OFF) {
    trigger.release();
  }
  safety_off();
  hopper_on();
  gun_armed = true;
  
}

void disarm_gun() {
  delay(50);
  Serial.println("disabled");
  safety_off();
  gun_off();
  safety_on();
  hopper_off();
  gun_armed = false;
}

void gun_on() {
  delay(100);
  digitalWrite(PIN_POWER, HIGH);
  delay(1860);
  digitalWrite(PIN_POWER, LOW);
  delay(100);
}

void gun_off() {
  delay(100);
  digitalWrite(PIN_POWER, HIGH);
  delay(2280);
  digitalWrite(PIN_POWER, LOW);
  delay(100);
}

void safety_off() {
  delay(100);
  digitalWrite(PIN_SAFETY_A, HIGH);
  digitalWrite(PIN_SAFETY_B, LOW);
  delay(100);
}

void safety_on() {
  delay(100);
  digitalWrite(PIN_SAFETY_A, HIGH);
  digitalWrite(PIN_SAFETY_B, HIGH);
  delay(100);
}

void hopper_on() {
  delay(100);
  digitalWrite(PIN_HOPPER, HIGH);
  delay(250);
  digitalWrite(PIN_HOPPER, LOW);
  delay(100);
}

void hopper_off() {
  delay(100);
  digitalWrite(PIN_HOPPER, HIGH);
  delay(1000);
  digitalWrite(PIN_HOPPER, LOW);
  delay(100);
}

void zero() {
  Serial.println("started calibration");
  pan.set_delay(0.5);
  tilt.set_delay(0.5);
  pan.move_relative(7000);
  tilt.move_relative(4000);
  pan.set_count(2900);
  tilt.set_count(2400);
  pan.move_absolute(0);
  tilt.move_absolute(0);
  delay(500);
  pan.set_delay(0.8);
  tilt.set_delay(0.8);
  pan.move_absolute(-2500);
  tilt.move_absolute(2000);
  pan.move_absolute(2500);
  tilt.move_absolute(-1000);
  pan.move_absolute(0);
  tilt.move_absolute(0);
  Serial.println("calibration complete");
}
