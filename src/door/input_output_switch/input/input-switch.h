#pragma once

class InputSwitch
{
public:
    enum class State
    {
        INPUT_LOW,
        INPUT_HIGH,
    };

    virtual ~InputSwitch() = default;
    virtual State get_state() = 0;
};

