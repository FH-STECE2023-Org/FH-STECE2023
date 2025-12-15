#pragma once
#include "analog-sensor.h"

class AnalogSensorMock : public AnalogSensor
{
public:
    AnalogSensorMock();
    float get_value() const override;
    // for tests
    void set_value(float value) const;
private:
    mutable float _value;
};
