#pragma once

#include <sys/timerfd.h>
#include "eventloop.h"
#include "timespec.h"

#include <functional>


class PeriodicTimer : public InputHandler
{
public:
    PeriodicTimer(TimeSpec set_time, std::function<void()>);
    ~PeriodicTimer() override;
    void hookup(Eventloop&);
    EventAction ready(int fd) override;

private:
    int _timer_fd;
    std::function<void()> _expired;
};
