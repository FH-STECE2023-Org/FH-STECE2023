#pragma once

#include "structs.h"
#include <memory>
#include <door/utilities/timespec.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>


class Inputs
{
public:
    Inputs(InputSwitch* button_outside, InputSwitch* button_inside, InputSwitch* light_barrier_closed, InputSwitch* light_barrier_open, AnalogSensorEventGenerator* Analogsensor, const TimeSpec& debounce_time);
    ~Inputs();

    // void check(const Events& events);
    input_t get_inputs();
    events_t get_events();

    
private:
    InputSwitch* _button_outside;
    InputSwitch* _button_inside;
    InputSwitch* _lightbarrier_closed;
    InputSwitch* _lightbarrier_open;
    AnalogSensorEventGenerator* _Analogsensor;

    std::shared_ptr<EdgeDetector> _edge_button_outside;
    std::shared_ptr<EdgeDetector> _edge_button_inside;
    std::shared_ptr<EdgeDetector> _edge_lightbarrier_closed;
    std::shared_ptr<EdgeDetector> _edge_lightbarrier_open;


};
