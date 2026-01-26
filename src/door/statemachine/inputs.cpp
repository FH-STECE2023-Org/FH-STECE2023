#include "inputs.h"
#include <door/utilities/event-edge-detector.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>
#include <memory>


Inputs::Inputs(std::shared_ptr<InputSwitch> button_outside, std::shared_ptr<InputSwitch> button_inside, std::shared_ptr<InputSwitch> lightbarrier_closed, std::shared_ptr<InputSwitch> lightbarrier_open, std::shared_ptr<AnalogSensorEventGenerator> analogsensor, const TimeSpec& debounce_time)
{
    _button_outside = button_outside;
    _button_inside = button_inside;
    _lightbarrier_closed = lightbarrier_closed;
    _lightbarrier_open = lightbarrier_open;
    _Analogsensor = analogsensor;

    _edge_button_outside = std::make_shared<EdgeDetector>(button_outside, debounce_time);
    _edge_button_inside = std::make_shared<EdgeDetector>(button_inside, debounce_time);
    _edge_lightbarrier_closed = std::make_shared<EdgeDetector>(lightbarrier_closed, debounce_time);
    _edge_lightbarrier_open = std::make_shared<EdgeDetector>(lightbarrier_open, debounce_time);
}


input_t Inputs::get_inputs()
{
    input_t input;
    input.button_outside = _button_outside->get_state();
    input.button_inside = _button_inside->get_state();
    input.sensor_closed = _lightbarrier_closed->get_state();
    input.sensor_open = _lightbarrier_open->get_state();
    input.analogsensor = _Analogsensor->get_event();
    return input;

}

events_t Inputs::get_events()
{
    events_t events;
    auto now = TimeSpec::now_monotonic();
    events.button_outside_pressed = _edge_button_outside->detect_edge(now);
    events.button_inside_pressed = _edge_button_inside->detect_edge(now);
    events.light_barrier_closed = _edge_lightbarrier_closed->detect_edge(now);
    events.light_barrier_open = _edge_lightbarrier_open->detect_edge(now);
    events.analog_state = _Analogsensor->get_event();
    return events;
}