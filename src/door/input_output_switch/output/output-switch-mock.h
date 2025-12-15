#pragma once

#include "output-switch.h"

class OutputSwitchMock : public OutputSwitch {
public:
    OutputSwitchMock();

    void set_state(State state) override;

    // Für Tests: Zustand abfragen
    State get_state() const;

private:
    State _state;
};