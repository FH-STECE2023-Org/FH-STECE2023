#pragma once
#include "analog-sensor.h"

enum class AnalogSensorEvent
{
    NORMAL_VALUE,
    UNDER_VALUE,
    OVER_VALUE,
    ANALOG_SENSOR_ERROR,
};

class AnalogSensorEventGenerator
{
    public:
        AnalogSensorEventGenerator(AnalogSensor* sensor, 
                                     float under_value = 0.0f, 
                                     float over_value = 0.0f,
                                     bool hysteresis_enabled = false,
                                     float hysteresis_value = 0.01f);

        // setters for hysteresis
        void enable_hysteresis(bool enable);
        void set_hysteresis_value(float value);

        // get_event updates internal state when hysteresis is enabled
        AnalogSensorEvent get_event();
    private:
        AnalogSensor* _sensor;
        float _under_value;
        float _over_value;
        
        // hysteresis configuration
        bool _hysteresis_enabled;
        float _hysteresis_value;

        // state for hysteresis behavior
        AnalogSensorEvent _last_event;
        bool _initialized;
};