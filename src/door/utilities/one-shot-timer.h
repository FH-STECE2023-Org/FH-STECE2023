#pragma once

#include <sys/timerfd.h>
#include "eventloop.h"
#include "timespec.h"

#include <functional>


class OneShotTimer : public InputHandler
{
public:
    OneShotTimer(TimeSpec set_time, std::function<void()>);
    ~OneShotTimer() override;
    void stop();
    void start();
    void hookup(Eventloop&);
    EventAction ready(int fd) override;
    bool isrunning(){ return _running; }

private:
    int _timer_fd;
    std::function<void()> _expired;
    TimeSpec _time;
    bool _running;
};
