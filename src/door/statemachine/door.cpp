#include "door.h"
#include <assert.h>

Door::Door()
{
    _state = State::INIT;
}

output_t Door::init(const input_t input)
{
    if ((input.sensor_closed == InputSwitch::State::INPUT_LOW) && (input.sensor_open == InputSwitch::State::INPUT_HIGH))
    {
        _state = State::CLOSED;
    }
    else
    {
        _state = State::ERROR_MIDDLE_POSITION;
    }

    output_t output;
    output.motor_direction = Motor::Direction::IDLE;

    return output;
}

output_t Door::cyclic(const events_t events)
{
    output_t output;

    switch(_state)
    {
        case State::INIT:

            output.motor_direction = Motor::Direction::IDLE;
            _state = State::ERROR_SOMETHING_BADLY_WRONG;
            break;

        case State::CLOSED:

            output.motor_direction = Motor::Direction::IDLE;

            // Gumminudel is broken
            if (events.analog_state == AnalogSensorEvent::UNDER_VALUE)
            {
                _state = State::ERROR_SOMETHING_BADLY_WRONG;
            }

            if (events.button_outside_pressed == EdgeDetector::RISING || events.button_inside_pressed == EdgeDetector::RISING)
            {
                _state = State::OPENING;
            }
            break;

        case State::OPENING:

            output.motor_direction = Motor::Direction::FORWARD; // could also be BACKWARD, depends on the implemented hardware

            // overcurrent detection -> ERROR

            // Gumminudel is broken
            if (events.analog_state == AnalogSensorEvent::UNDER_VALUE)
            {
                _state = State::ERROR_SOMETHING_BADLY_WRONG;
            }

            if (events.light_barrier_open == EdgeDetector::FALLING)
            {
                _state = State::OPENED;
            }
            break;

        case State::OPENED:

            output.motor_direction = Motor::Direction::IDLE;

            // Gumminudel is broken
            if (events.analog_state == AnalogSensorEvent::UNDER_VALUE)
            {
                _state = State::ERROR_SOMETHING_BADLY_WRONG;
            }

            if (events.button_outside_pressed == EdgeDetector::RISING || events.button_inside_pressed == EdgeDetector::RISING)
            {
                _state = State::CLOSING;
            }
        break;

        case State::CLOSING:

            output.motor_direction = Motor::Direction::BACKWARD; // could also be BACKWARD, depends on the implemented hardware

            // Kindskopf detected with Gumminudel -> ERROR
            if (events.analog_state == AnalogSensorEvent::OVER_VALUE)
            {
                _state = State::OPENING;
            }

            // Gumminudel is broken
            if (events.analog_state == AnalogSensorEvent::UNDER_VALUE)
            {
                _state = State::ERROR_SOMETHING_BADLY_WRONG;
            }

            if (events.light_barrier_closed == EdgeDetector::FALLING)
            {
                _state = State::CLOSED;
            }           
        break;

        case State::ERROR_SOMETHING_BADLY_WRONG:

            output.motor_direction = Motor::Direction::IDLE;
            // Feature request -> Light beacon to show error state.
        break;

        default:
            break;
    }

    return output;
}