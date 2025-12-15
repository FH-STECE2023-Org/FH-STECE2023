#include "outputs.h"
#include "motor.h"

Outputs::Outputs(Motor* motor)
{
    _motor = motor;
}

void Outputs::set_outputs(const output_t out)
{
    switch(out.motor_direction)
    {
        case Motor::Direction::FORWARD:
            _motor->forward();
            break;

        case Motor::Direction::BACKWARD:
            _motor->backward();
            break;

        case Motor::Direction::IDLE:
            _motor->stop();
            break;
    }
}