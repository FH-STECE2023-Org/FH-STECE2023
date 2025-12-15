#include "inputs.h"
#include "event-edge-detector.h"
#include "analog-sensor-event-generator.h"


Inputs::Inputs(InputSwitch* button_outside, InputSwitch* button_inside, InputSwitch* lightbarrier_closed, InputSwitch* lightbarrier_open, AnalogSensorEventGenerator* analogsensor, const TimeSpec& debounce_time)
{
    _button_outside = button_outside;
    _button_inside = button_inside;
    _lightbarrier_closed = lightbarrier_closed;
    _lightbarrier_open = lightbarrier_open;
    _Analogsensor = analogsensor;

    _edge_button_outside = new EdgeDetector(button_outside, debounce_time);
    _edge_button_inside = new EdgeDetector(button_inside, debounce_time);
    _edge_lightbarrier_closed = new EdgeDetector(lightbarrier_closed, debounce_time);
    _edge_lightbarrier_open = new EdgeDetector(lightbarrier_open, debounce_time);
}

Inputs::~Inputs()
{
    delete _edge_button_outside;
    delete _edge_button_inside;
    delete _edge_lightbarrier_closed;
    delete _edge_lightbarrier_open;
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