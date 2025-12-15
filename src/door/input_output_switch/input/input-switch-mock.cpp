#include "input-switch-mock.h"


InputSwitchMock::InputSwitchMock(State state)
: _state{state} {}

InputSwitch::State InputSwitchMock::get_state()
{
    return _state;
}

void InputSwitchMock::set_state(State state)
{
    _state = state;
}
