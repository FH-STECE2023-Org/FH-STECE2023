#pragma once

#include <door/input_output_switch/input/input-switch.h>
#include "timespec.h"
#include <memory>

class EdgeDetector
{
public:
    enum State
    {
        RISING,
        FALLING,
        NONE
    };

    EdgeDetector(std::shared_ptr<InputSwitch> input, const TimeSpec& debounce_time);
    State detect_edge(const TimeSpec& now);

private:
    InputSwitch::State _old_state;
    std::shared_ptr<InputSwitch> _input;
    State _state;

    TimeSpec _debounce;
    TimeSpec _old_time;
};