#include "output-switch-mock.h"

OutputSwitchMock::OutputSwitchMock()
    : _state(State::OUTPUT_LOW) {}

void OutputSwitchMock::set_state(State state)
{
    _state = state;
}

OutputSwitch::State OutputSwitchMock::get_state() const 
{
    return _state;
}