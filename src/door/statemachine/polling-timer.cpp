#include "polling-timer.h"

PollingTimer::PollingTimer(Inputs& inputs, Outputs& outputs, Door& door)
    : _inputs(inputs), _outputs(outputs), _door(door)
{}


void PollingTimer::expired()
{
    events_t ev  = _inputs.get_events();
    output_t out = _door.cyclic(ev);

    _outputs.set_outputs(out);
}