#pragma once

#include "motor.h"
#include <string>
#include "output-switch-gpio-sysfs.h"

class MotorStepper : public Motor
{
    public:
        MotorStepper();
        ~MotorStepper();

        void forward() override;
        void backward() override;
        void stop() override;
        Direction get_direction() const override;


    private:
        int writeData(std::string path, std::string value);
        int ensureExported();

        OutputSwitchGPIOSysfs _line_enable;
        OutputSwitchGPIOSysfs _line_direction;
        
        Direction _direction;
        std::string _period_nanosec;
        std::string _duty_nanosec;
};