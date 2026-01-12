#include "periodic-timer.h"

#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdexcept>
#include <stdint.h>

PeriodicTimer::PeriodicTimer(TimeSpec set_time, std::function<void()> expired) 
: _expired(expired)
{
    _timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (_timer_fd == -1) {
        perror("timerfd_create");
        throw std::runtime_error("timerfd_create failed");
    }
    itimerspec spec{
        .it_interval = {set_time.tv_sec, set_time.tv_nsec},   // periodic: every 1ms
        .it_value    = {0, 1},   //almost immediate expire
    };

    if (timerfd_settime(_timer_fd, 0, &spec, nullptr) == -1) {
        perror("timerfd_settime");
        throw std::runtime_error("timerfd_settime failed");
    }
}

PeriodicTimer::~PeriodicTimer()
{
    if (_timer_fd != -1)
        close(_timer_fd);
}

void PeriodicTimer::hookup(Eventloop& loop)
{
    loop.register_input(_timer_fd, this);
}

EventAction PeriodicTimer::ready(int fd)
{
    if (fd == _timer_fd) {
        uint64_t expirations;
        ssize_t n = read(_timer_fd, &expirations, sizeof(expirations));

        if (n != sizeof(expirations)) {
            return EventAction::Continue;  
        }
        _expired();

        return EventAction::Continue;
    }

    return EventAction::Continue;
}

