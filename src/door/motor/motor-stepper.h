#pragma once

#include "motor.h"
#include <string>
#include "output-switch.h"

class MotorStepper : public Motor
{
    public:
        MotorStepper(const std::string& gpiodevice, OutputSwitch& line_enable, OutputSwitch& line_direction, std::string period_nanosec, std::string duty_nanosec);
        ~MotorStepper();

        void forward() override;
        void backward() override;
        void stop() override;
        Direction get_direction() const override;


    private:
        int writeData(std::string path, std::string value);
        int ensureExported();

        OutputSwitch& _line_enable;
        OutputSwitch& _line_direction;
        
        Direction _direction;
        std::string _period_nanosec;
        std::string _duty_nanosec;
};