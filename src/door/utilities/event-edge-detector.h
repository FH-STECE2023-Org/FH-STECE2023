#pragma once

#include <door/input_output_switch/input/input-switch.h>
#include "timespec.h"

class EdgeDetector
{
public:
    enum State
    {
        RISING,
        FALLING,
        NONE
    };

    EdgeDetector(InputSwitch* input, const TimeSpec& debounce_time);
    State detect_edge(const TimeSpec& now);

private:
    InputSwitch::State _old_state;
    InputSwitch* _input;
    State _state;

    TimeSpec _debounce;
    TimeSpec _old_time;
};