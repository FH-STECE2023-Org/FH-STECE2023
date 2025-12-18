#include <door/statemachine/door.h>
#include <door/input_output_switch/input/input-switch.h>
#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdlib.h>

TEST(door_init_suite, door_init)
{
    // create Door object
    Door door;

    // create Input struct
    input_t input;
    // Assumption -> limit switches are active low
    input.sensor_closed = InputSwitch::State::INPUT_LOW;
    input.sensor_open = InputSwitch::State::INPUT_HIGH;

    // create Output struct
    output_t output;

    // run door.init
    output = door.init(input);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::CLOSED);
    ASSERT_EQ(output.motor_direction, Motor::Direction::IDLE);
}

TEST(door_init_suite, door_init_error)
{
    // create Door object
    Door door;

    // create Input struct
    input_t input;
    // Assumption -> limit switches are active low
    input.sensor_closed = InputSwitch::State::INPUT_HIGH;
    input.sensor_open = InputSwitch::State::INPUT_HIGH;

    // create Output struct
    output_t output;

    // run door.init
    output = door.init(input);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::ERROR_MIDDLE_POSITION);
    ASSERT_EQ(output.motor_direction, Motor::Direction::IDLE);
}