#include "one-shot-timer.h"

#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdexcept>
#include <stdint.h>

OneShotTimer::OneShotTimer(TimeSpec set_time, std::function<void()> expired) 
: _expired(expired), _time(set_time)
{
    _timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (_timer_fd == -1) {
        perror("timerfd_create");
        throw std::runtime_error("timerfd_create failed");
    }
}

void OneShotTimer::stop()
{
    itimerspec spec{
        .it_interval = {0, 0},
        .it_value    = {0, 0}, 
    };

    if (timerfd_settime(_timer_fd, 0, &spec, nullptr) == -1) {
        perror("timerfd_settime");
        throw std::runtime_error("timerfd_settime failed");
    }
    else{ _running = false; }
}

void OneShotTimer::start()
{
    itimerspec spec{
        .it_interval = {0, 0},
        .it_value    = {_time.tv_sec, _time.tv_nsec},   // One-Shot
    };

    if (timerfd_settime(_timer_fd, 0, &spec, nullptr) == -1) {
        perror("timerfd_settime");
        throw std::runtime_error("timerfd_settime failed");
    }
    else{ _running = true; }
}

void OneShotTimer::hookup(Eventloop& loop)
{
    loop.register_input(_timer_fd, this);
}

EventAction OneShotTimer::ready(int fd)
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

OneShotTimer::~OneShotTimer()
{
    if(!_running)
    {
        if (_timer_fd != -1)
            close(_timer_fd);
    }
    else{
        throw std::runtime_error("OneShot-Timer still running. Destruction failed! :'(");
    }
}

