#pragma once

#include "structs.h"


class Door
{
public:
    enum State
    {
        INIT,
        CLOSING,
        CLOSED,
        OPENING,
        OPENED,
        ERROR_MIDDLE_POSITION,
        ERROR_SOMETHING_BADLY_WRONG,
    };

    Door();
    
    // void check(const Events& events);
    output_t init(const input_t input);
    output_t cyclic(const events_t event);

    // for tests only
    void set_state(State state) { _state = state; }
    State get_state() const { return _state; }

private:
    State _state;
};
