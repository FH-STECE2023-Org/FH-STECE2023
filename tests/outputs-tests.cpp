#include <door/statemachine/outputs.h>
#include <door/statemachine/door.h>
#include <door/motor/motor-mock.h>
#include <gtest/gtest.h>
#include <stdbool.h>

TEST(output_suite, motor_forward)
{
    // create output
    MotorMock motor(Motor::Direction::IDLE);
    Outputs output(&motor);
    output_t out;

    // set output
    out.motor_direction = Motor::Direction::FORWARD;
    output.set_outputs(out);

    // test actors
    ASSERT_EQ(motor.get_direction(), Motor::Direction::FORWARD);
}

TEST(output_suite, motor_backward)
{
    // create output
    MotorMock motor(Motor::Direction::IDLE);
    Outputs output(&motor);
    output_t out;

    // set output
    out.motor_direction = Motor::Direction::BACKWARD;
    output.set_outputs(out);

    // test actors
    ASSERT_EQ(motor.get_direction(), Motor::Direction::BACKWARD);
}

TEST(output_suite, motor_stop)
{
    // create output
    MotorMock motor(Motor::Direction::IDLE);
    Outputs output(&motor);
    output_t out;

    // set output
    out.motor_direction = Motor::Direction::IDLE;
    output.set_outputs(out);

    // test actors
    ASSERT_EQ(motor.get_direction(), Motor::Direction::IDLE);
}