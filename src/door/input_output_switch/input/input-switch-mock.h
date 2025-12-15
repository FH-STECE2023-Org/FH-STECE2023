#pragma once

#include "input-switch.h"

class InputSwitchMock : public InputSwitch
{
public:
    InputSwitchMock(State state);
    virtual State get_state();

    // for tests only (puropsely not part of the interface)
    void set_state(State state);

private:
    State _state;
};

