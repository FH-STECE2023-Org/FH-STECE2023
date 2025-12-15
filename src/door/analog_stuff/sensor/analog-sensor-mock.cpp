#include "analog-sensor-mock.h"

AnalogSensorMock::AnalogSensorMock() : _value {}{}

float AnalogSensorMock::get_value() const
{
    return _value;
}

void AnalogSensorMock::set_value(float value) const
{
    _value = value;
}
