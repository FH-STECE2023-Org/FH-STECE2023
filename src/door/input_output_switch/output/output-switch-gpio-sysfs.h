#pragma once

#include "output-switch.h"
#include <string>

class OutputSwitchGPIOSysfs : public OutputSwitch {
public:
    OutputSwitchGPIOSysfs(unsigned int line_number);
    ~OutputSwitchGPIOSysfs();

    void set_state(State state) override;

private:
    unsigned int _line_number;
};