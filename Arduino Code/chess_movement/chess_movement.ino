/*

1) Read five integers from serial (pos1, pos2, kill_target)
2) Determine whether straight, diagonal, or knight
3) Use absolute stepping to get to origin square
4) Use relative stepping to get to target square (w/ appropriate movement)
5) [Send signal back to serial]

Improvements:

1) Move piece off at better time
2) Make dead pieces form a line

*/

#include "RL_Stepper.h"

#define STEPS_SQUARE 400
#define STEPS_HALF_SQUARE (STEPS_SQUARE / 2)

#define row_step_pin 5
#define row_direction_pin 4
#define row_enable_pin 13

#define col_step_pin 3
#define col_direction_pin 2
#define col_enable_pin 14

#define motor_delay 2

#define magnet_pin 10

#define limit_row 8
#define limit_col 9

#define dead_zone 1700


Stepper row_motor(row_step_pin, row_direction_pin, row_enable_pin);
Stepper col_motor(col_step_pin, col_direction_pin, col_enable_pin);

void setup()
{
    Serial.begin(9600);

    row_motor.enable();
    row_motor.set_delay(motor_delay);

    col_motor.enable();
    col_motor.set_delay(motor_delay);

    pinMode(magnet_pin, OUTPUT);

    // Enable internal pullup resistor
    pinMode(limit_row, INPUT);
    pinMode(limit_col, INPUT);
    digitalWrite(limit_row, HIGH);
    digitalWrite(limit_col, HIGH);

    digitalWrite(magnet_pin, HIGH);
    calibrate();
}

void loop()
{
    if (Serial.available())
    {
        int row1 = Serial.parseInt();
        int col1 = Serial.parseInt();
        int row2 = Serial.parseInt();
        int col2 = Serial.parseInt();
        int kill_target = Serial.parseInt();

        if (kill_target)
        {
            kill_piece(row2, col2);
        }

        int row_move = row2 - row1;
        int col_move = col2 - col1;

        row_motor.move_absolute(row1 * STEPS_SQUARE);
        col_motor.move_absolute(col1 * STEPS_SQUARE);

        if (row_move == 0 || col_move == 0) // Straight
        {
            move_straight(row_move, col_move);
        }

        else if (abs(row_move) == abs(col_move)) // Diagonal
        {
            move_diagonal(row_move, col_move);
        }

        else // Knight
        {
            move_knight(row_move, col_move);
        }

    }

}

void move_straight(int row_move, int col_move)
{
    digitalWrite(magnet_pin, HIGH);
    row_motor.move_relative(row_move * STEPS_SQUARE);
    col_motor.move_relative(col_move * STEPS_SQUARE);
    digitalWrite(magnet_pin, LOW);
}

void move_diagonal(int row_move, int col_move)
{
    int num_steps = abs(row_move) * STEPS_SQUARE;
    int row_sign = sign(row_move);
    int col_sign = sign(col_move);

    digitalWrite(magnet_pin, HIGH);

    for (int i = 0; i < num_steps; i++)
    {
        row_motor.move_relative(row_sign);
        col_motor.move_relative(col_sign);
    }

    digitalWrite(magnet_pin, LOW);

}

void move_knight(int row_move, int col_move)
{
    digitalWrite(magnet_pin, HIGH);

    // Move to corner of square, since knight must move along edges
    row_motor.move_relative(-STEPS_HALF_SQUARE);
    col_motor.move_relative(-STEPS_HALF_SQUARE);

    row_motor.move_relative(row_move * STEPS_SQUARE);
    col_motor.move_relative(col_move * STEPS_SQUARE);

    row_motor.move_relative(STEPS_HALF_SQUARE);
    col_motor.move_relative(STEPS_HALF_SQUARE);

    digitalWrite(magnet_pin, LOW);
}

void kill_piece(int row, int col)
{
    row_motor.move_absolute(row * STEPS_SQUARE);
    col_motor.move_absolute(col * STEPS_SQUARE);

    digitalWrite(magnet_pin, HIGH);

    col_motor.move_relative(-STEPS_HALF_SQUARE);
    row_motor.move_absolute(dead_zone);

    digitalWrite(magnet_pin, LOW);
}


void calibrate()
{
    // Hit both limit switches, then move to center of Azkaban 1!
    
    while (digitalRead(limit_row) == HIGH)
    {
        row_motor.move_relative(-1);
    }
    row_motor.move_relative(475);
    row_motor.set_count(0);
    
    while (digitalRead(limit_col) == HIGH)
    {
        col_motor.move_relative(-1);
    }
    col_motor.move_relative(525);
    col_motor.set_count(0);
}

int sign(int x) { return x < 0 ? -1 : 1; }
