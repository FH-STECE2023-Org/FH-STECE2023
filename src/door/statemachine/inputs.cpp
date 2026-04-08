#include "inputs.h"
#include <door/utilities/event-edge-detector.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>


Inputs::Inputs(InputSwitch* button_outside, InputSwitch* button_inside, InputSwitch* sensor_closed, InputSwitch* sensor_open, AnalogSensorEventGenerator* analogsensor, const TimeSpec& debounce_time)
{
    _button_outside = button_outside;
    _button_inside = button_inside;
    _sensor_closed = sensor_closed;
    _sensor_open = sensor_open;
    _Analogsensor = analogsensor;

    _edge_button_outside = new EdgeDetector(button_outside, debounce_time);
    _edge_button_inside = new EdgeDetector(button_inside, debounce_time);
    _edge_sensor_closed = new EdgeDetector(sensor_closed, debounce_time);
    _edge_sensor_open = new EdgeDetector(sensor_open, debounce_time);
}

Inputs::~Inputs()
{
    delete _edge_button_outside;
    delete _edge_button_inside;
    delete _edge_sensor_closed;
    delete _edge_sensor_open;
}

input_t Inputs::get_inputs()
{
    input_t input;
    input.button_outside = _button_outside->get_state();
    input.button_inside = _button_inside->get_state();
    input.sensor_closed = _sensor_closed->get_state();
    input.sensor_open = _sensor_open->get_state();
    input.analogsensor = _Analogsensor->get_event();
    return input;

}

events_t Inputs::get_events()
{
    events_t events;
    auto now = TimeSpec::now_monotonic();
    events.button_outside_pressed = _edge_button_outside->detect_edge(now);
    events.button_inside_pressed = _edge_button_inside->detect_edge(now);
    events.sensor_closed = _edge_sensor_closed->detect_edge(now);
    events.sensor_open = _edge_sensor_open->detect_edge(now);
    events.analog_state = _Analogsensor->get_event();
    return events;
}