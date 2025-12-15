#include "eventloop.h"

#include <vector>
#include <stdexcept>
#include <cassert>

#include <string.h>
#include <poll.h>


void Eventloop::register_input(int fd, InputHandler* h)
{
    const auto& [_, inserted] = _input_handlers.insert(std::make_pair(fd, h));
    assert(inserted);                                  // <-- don't watch an fd twice
}

void Eventloop::register_priority(int fd, PriorityHandler* h)
{
    const auto& [_, inserted] = _priority_handlers.insert(std::make_pair(fd, h));
    assert(inserted);                                  // <-- don't watch an fd twice
}
void Eventloop::run()
{
    while (run_one() != EventAction::Quit);
}

EventAction Eventloop::run_one()
{
    std::map</*fd*/int, /*poll-flags*/short> collected;
    for (auto [fd, h]: _input_handlers) {
        auto [elem, inserted] = collected.insert(std::make_pair(fd, POLLIN));
        if (!inserted)
            elem->second |= POLLIN;
    }
    for (auto [fd, h]: _priority_handlers) {
        auto [elem, inserted] = collected.insert(std::make_pair(fd, POLLPRI));
        if (!inserted)
            elem->second |= POLLPRI;
    }

    std::vector<struct pollfd> watches;
    watches.reserve(collected.size());
    for (auto [fd, flags]: collected) {
        watches.push_back({
            .fd = fd,
            .events = flags,
        });
    }

    int nready = poll(&watches[0], watches.size(), -1);
    if (nready == -1)
        throw std::runtime_error("nix gut, das ist eine schlechte Fehlermeldung, blame jfasch");
    if (nready == 0)
        throw std::runtime_error("poll returns 0 though no timeout requested");

    EventAction action = EventAction::Continue;
    for (const auto& watch: watches) {
        if (watch.revents & POLLIN)
            switch (_input_handlers[watch.fd]->ready(watch.fd)) {
                case EventAction::Quit: 
                    action = EventAction::Quit;
                    break;
                case EventAction::Continue:
                    break;
            }
        if (watch.revents & POLLPRI)
            switch (_priority_handlers[watch.fd]->ready(watch.fd)) {
                case EventAction::Quit: 
                    action = EventAction::Quit;
                    break;
                case EventAction::Continue:
                    break;
            }
    }

    return action;
}
