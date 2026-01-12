#include "input-edge-event.h"

#include <door/utilities/small_file_handling.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdexcept>
#include <sstream>
#include <cerrno>
#include <iostream>

InputEdgeEvent::InputEdgeEvent(int pin, const std::string& edge, const EventAction event)
: _pin(pin), _fd(-1), _event(event)
{
    std::ostringstream oss;
    oss << "/sys/class/gpio/gpio" << pin;
    std::string base = oss.str();

    const std::string gpio_str = std::to_string(pin);
    const std::string direction_path = base + "/direction";

    // Same initialization pattern as InputSwitchGPIOSysfs
    if (dir_exists(direction_path) == false) {
        write_string_to_file("/sys/class/gpio/export", gpio_str);
        usleep(100000);
    }
    
    // 2. Set Direction Input
    write_string_to_file(direction_path, "in");

    // 3. Set Edge Trigger
    write_string_to_file(base + "/edge", edge);

    // 4. Open Value (store fd for the event loop)
    _fd = open((base + "/value").c_str(), O_RDONLY | O_NONBLOCK);
    if (_fd == -1) {
        std::ostringstream oss;
        oss << "open gpio value failed: " << strerror(errno);
        throw std::runtime_error(oss.str());
    }
}

InputEdgeEvent::~InputEdgeEvent()
{
    if (_fd != -1) close(_fd);

    try {
        write_string_to_file("/sys/class/gpio/unexport", std::to_string(_pin));
    } catch (const std::exception& e) {
        std::cerr << "InputEdgeEvent: failed to unexport GPIO " << _pin << ": " << e.what() << "\n";
    } catch (...) {
        std::cerr << "InputEdgeEvent: failed to unexport GPIO " << _pin << ": unknown error\n";
    }
}

void InputEdgeEvent::hookup(Eventloop& loop)
{
    loop.register_priority(_fd, this);
}

EventAction InputEdgeEvent::ready(int fd)
{
    //Todo: Find a way to decouple event for EventLoop and events for "door" 
    //Implement a interface that abstracts the Event here to for the door logic (probably connects to inputsclass)?
    // Sysfs edge events: clear the pending POLLPRI by seeking + reading the value.
    // If we only lseek(), POLLPRI can stay asserted and the loop spins.
    lseek(fd, 0, SEEK_SET);
    char buf[8];
    (void)read(fd, buf, sizeof(buf));

    // ???

    return _event; // Continue
}
