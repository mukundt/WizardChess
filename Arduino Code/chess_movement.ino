/*

1) Read two integers from serial
2) Determine whether straight, diagonal, or knight
3) Use absolute stepping to get to origin square
4) Use relative stepping to get to target square (w/ appropriate movement)
5) [Send signal back to serial]

*/

#include "RL_Stepper.h"

#define STEPS_SQUARE 200
#define STEPS_HALF_SQUARE (STEPS_SQUARE / 2)

Stepper row_motor(row_step_pin, row_direction_pin, row_enable_pin);
Stepper col_motor(col_step_pin, col_direction_pin, col_enable_pin);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available())
    {
        int row1 = Serial.parseInt();
        int col1 = Serial.parseInt();
        int row2 = Serial.parseInt();
        int col2 = Serial.parseInt();


        int row_move = row2 - row1;
        int col_move = col2 - col1;

        row_motor.move_absolute(row1 * STEPS_SQUARE);
        col_motor.move_absolute(col1 * STEPS_SQUARE);
    }

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

void move_straight(int row_move, int col_move)
{
    row_motor.move_relative(row_move * STEPS_SQUARE);
    col_motor.move_relative(col_move * STEPS_SQUARE);
}

void move_diagonal(int row_move, int col_move)
{
    int num_steps = abs(row_move) * STEPS_SQUARE;
    int row_sign = sign(row_move);
    int col_sign = sign(col_move);

    for (int i = 0; i < num_steps; i++)
    {
        row_motor.single_step(row_sign);
        col_motor.single_step(col_sign);
    }

}

void move_knight(int row_move, int col_move)
{
    // Move to corner of square, since knight must move along edges
    row_motor.move_relative(-STEPS_HALF_SQUARE);
    col_motor.move_relative(-STEPS_HALF_SQUARE);

    row_motor.move_relative(row_move * STEPS_SQUARE);
    col_motor.move_relative(col_move * STEPS_SQUARE);

    row_motor.move_relative(STEPS_HALF_SQUARE);
    col_motor.move_relative(STEPS_HALF_SQUARE);
}

int sign(int x) { return x < 0 ? -1 : 1; }
