#pragma once

#include "input-switch.h"
#include <string.h>

class InputSwitchGPIOSysfs : public InputSwitch {
    public:
        InputSwitchGPIOSysfs(unsigned int line_number);
        ~InputSwitchGPIOSysfs();

        State get_state() override;

    private:
        unsigned int _line_number;
};
