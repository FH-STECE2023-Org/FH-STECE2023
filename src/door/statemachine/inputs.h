#pragma once

#include "structs.h"
#include <door/utilities/timespec.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>


class Inputs
{
public:
    Inputs(InputSwitch* button_outside, InputSwitch* button_inside, InputSwitch* sensor_closed, InputSwitch* sensor_open, AnalogSensorEventGenerator* Analogsensor, const TimeSpec& debounce_time);
    ~Inputs();

    // void check(const Events& events);
    input_t get_inputs();
    events_t get_events();

    
private:
    InputSwitch* _button_outside;
    InputSwitch* _button_inside;
    InputSwitch* _sensor_closed;
    InputSwitch* _sensor_open;
    AnalogSensorEventGenerator* _Analogsensor;

    EdgeDetector *_edge_button_outside;
    EdgeDetector *_edge_button_inside;
    EdgeDetector *_edge_sensor_closed;
    EdgeDetector *_edge_sensor_open;
};
