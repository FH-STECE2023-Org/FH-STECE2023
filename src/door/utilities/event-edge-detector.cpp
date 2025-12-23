#include "event-edge-detector.h"

EdgeDetector::EdgeDetector(InputSwitch* input, const TimeSpec& debounce_time)
{
    _input = input;
    _old_state = _input->get_state();
    _state = EdgeDetector::State::NONE;
    _debounce = debounce_time;
}

EdgeDetector::State EdgeDetector::detect_edge(const TimeSpec& now)
{
    InputSwitch::State now_state = _input->get_state();
    EdgeDetector::State edge;
    if(now - _old_time >= _debounce)
    {
        if(_old_state == InputSwitch::State::INPUT_LOW && now_state == InputSwitch::State::INPUT_HIGH)
        {
            edge = EdgeDetector::State::RISING;
        }
        else if(_old_state == InputSwitch::State::INPUT_HIGH && now_state == InputSwitch::State::INPUT_LOW)
        {
            edge = EdgeDetector::State::FALLING;
        }
        else
        {
            edge = EdgeDetector::State::NONE;
        }
        _old_state = now_state;
        _old_time = now;
        return edge;
    }
    else
    {
        return EdgeDetector::State::NONE;
    }
}
