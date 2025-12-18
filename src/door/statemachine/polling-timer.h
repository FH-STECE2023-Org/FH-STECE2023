#pragma once

#include <sys/timerfd.h>
#include "inputs.h"
#include "outputs.h"
#include "door.h"
#include "structs.h"
#include <door/utilities/timer-expired.h>

class PollingTimer : public TimerExpired
{
public: 
    PollingTimer(Inputs& inputs, Outputs& outputs, Door& door);
    void expired() override;

private:
    Inputs& _inputs;
    Outputs& _outputs;
    Door& _door;
    events_t _ev;
    output_t _out;
};