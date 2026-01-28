#pragma once

#include "structs.h"
#include <memory>
#include <door/utilities/timespec.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>


class Inputs
{
public:
    Inputs(std::shared_ptr<InputSwitch> button_outside, std::shared_ptr<InputSwitch> button_inside, std::shared_ptr<InputSwitch> light_barrier_closed, std::shared_ptr<InputSwitch> light_barrier_open, std::shared_ptr<AnalogSensorEventGenerator> Analogsensor, const TimeSpec& debounce_time);
    ~Inputs() = default;

    // void check(const Events& events);
    input_t get_inputs();
    events_t get_events();

    
private:
    std::shared_ptr<InputSwitch> _button_outside;
    std::shared_ptr<InputSwitch> _button_inside;
    std::shared_ptr<InputSwitch> _lightbarrier_closed;
    std::shared_ptr<InputSwitch> _lightbarrier_open;
    std::shared_ptr<AnalogSensorEventGenerator> _Analogsensor;

    std::shared_ptr<EdgeDetector> _edge_button_outside;
    std::shared_ptr<EdgeDetector> _edge_button_inside;
    std::shared_ptr<EdgeDetector> _edge_lightbarrier_closed;
    std::shared_ptr<EdgeDetector> _edge_lightbarrier_open;


};
