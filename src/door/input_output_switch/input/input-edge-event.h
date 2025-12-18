#pragma once

#include <door/utilities/eventloop.h>
#include <string>

class InputEdgeEvent : public PriorityHandler
{
public:
    // edge: "rising", "falling", "both"
    InputEdgeEvent(int pin, const std::string& edge, const EventAction event);
    ~InputEdgeEvent() override;

    void hookup(Eventloop&);
    EventAction ready(int fd) override;

    int pin() const { return _pin; }

private:
    int _pin;
    int _fd;
    EventAction _event;
};