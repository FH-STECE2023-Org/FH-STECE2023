#include <door/statemachine/door.h>
#include <door/input_output_switch/input/input-switch.h>
#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdlib.h>

// Transition from "CLOSED" to "OPENING" with button_outside
TEST(door_opening_suite, door_opening_button_outside_pressed)
{
    // create Door object
    Door door;
    door.set_state(Door::State::CLOSED);

    // create events struct
    events_t events;
    events.analog_state = AnalogSensorEvent::NORMAL_VALUE;
    events.button_outside_pressed = EdgeDetector::RISING;
    events.button_inside_pressed = EdgeDetector::NONE;
    events.light_barrier_closed = EdgeDetector::NONE;
    events.light_barrier_open = EdgeDetector::NONE;

    // create output struct
    output_t output;

    // run door.cyclic
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::OPENING);
    ASSERT_EQ(output.motor_direction, Motor::Direction::IDLE);

    // button change is none
    events.button_outside_pressed = EdgeDetector::NONE;

    // call door again
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::OPENING);
    ASSERT_EQ(output.motor_direction, Motor::Direction::FORWARD);
}

// Transition from "CLOSED" to "OPENING" with button_inside
TEST(door_opening_suite, door_opening_button_inside_pressed)
{
    // create Door object
    Door door;
    door.set_state(Door::State::CLOSED);

    // create events struct
    events_t events;
    events.analog_state = AnalogSensorEvent::NORMAL_VALUE;
    events.button_outside_pressed = EdgeDetector::NONE;
    events.button_inside_pressed = EdgeDetector::RISING;
    events.light_barrier_closed = EdgeDetector::NONE;
    events.light_barrier_open = EdgeDetector::NONE;

    // create output struct
    output_t output;

    // run door.cyclic
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::OPENING);
    ASSERT_EQ(output.motor_direction, Motor::Direction::IDLE);

    // button change is none
    events.button_inside_pressed = EdgeDetector::NONE;

    // call door again
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::OPENING);
    ASSERT_EQ(output.motor_direction, Motor::Direction::FORWARD);
}

// Transition from "OPENING" to "OPEN"
TEST(door_opening_suite, door_open_reached)
{
    //create Door object
    Door door;
    door.set_state(Door::State::OPENING);

    // create events struct
    events_t events;
    events.analog_state = AnalogSensorEvent::NORMAL_VALUE;
    events.button_inside_pressed = EdgeDetector::NONE;
    events.button_outside_pressed = EdgeDetector::NONE;
    events.light_barrier_closed = EdgeDetector::NONE;
    events.light_barrier_open = EdgeDetector::FALLING;

    // create output struct
    output_t output;

    // run door.cyclic
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::OPENED);
    ASSERT_EQ(output.motor_direction, Motor::Direction::FORWARD);

    // run again
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::OPENED);
    ASSERT_EQ(output.motor_direction, Motor::Direction::IDLE);
}

// ------------------------------------------------------------------------------------
// --------------------------- Error states during "OPENING" --------------------------
// ------------------------------------------------------------------------------------

// Gumminudel broken in "OPENING"
TEST(door_opening_error_suite, door_opening_gumminudel_broken)
{
    //create Door object
    Door door;
    door.set_state(Door::State::OPENING);

    // create events struct
    events_t events;
    events.analog_state = AnalogSensorEvent::UNDER_VALUE;
    events.button_inside_pressed = EdgeDetector::NONE;
    events.button_outside_pressed = EdgeDetector::NONE;
    events.light_barrier_closed = EdgeDetector::NONE;
    events.light_barrier_open = EdgeDetector::NONE;

    // create output struct
    output_t output;

    // run door.cyclic
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::ERROR_SOMETHING_BADLY_WRONG);
    ASSERT_EQ(output.motor_direction, Motor::Direction::FORWARD);

    // run door.cyclic
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::ERROR_SOMETHING_BADLY_WRONG);
    ASSERT_EQ(output.motor_direction, Motor::Direction::IDLE);
}

// Gumminudel broken in "OPENED"
TEST(door_opening_error_suite, door_open_gumminudel_broken)
{
    //create Door object
    Door door;
    door.set_state(Door::State::OPENED);

    // create events struct
    events_t events;
    events.analog_state = AnalogSensorEvent::UNDER_VALUE;
    events.button_inside_pressed = EdgeDetector::NONE;
    events.button_outside_pressed = EdgeDetector::NONE;
    events.light_barrier_closed = EdgeDetector::NONE;
    events.light_barrier_open = EdgeDetector::NONE;

    // create output struct
    output_t output;

    // run door.cyclic
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::ERROR_SOMETHING_BADLY_WRONG);
    ASSERT_EQ(output.motor_direction, Motor::Direction::IDLE);

    // run door.cyclic
    output = door.cyclic(events);

    // check state and output
    ASSERT_EQ(door.get_state(), Door::State::ERROR_SOMETHING_BADLY_WRONG);
    ASSERT_EQ(output.motor_direction, Motor::Direction::IDLE);
}