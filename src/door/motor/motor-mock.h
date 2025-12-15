#pragma once

#include "motor.h"


class MotorMock : public Motor
{
public:
    MotorMock(Direction direction);
    void forward() override;
    void backward() override;
    void stop() override;

    // for tests only? (Door uses for a sanity check)
    Direction get_direction() const override;

private:
    Direction _direction;
};

