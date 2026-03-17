#include <cstdio>
#include <door/analog_stuff/sensor/analog-sensor-mock.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>
#include <door/utilities/eventloop.h>

#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <functional>
#include <utility>
#include <stdexcept>

// Convert enum to string for logging
const char* event_to_string(AnalogSensorEvent ev)
{
    switch (ev) {
        case AnalogSensorEvent::NORMAL_VALUE: return "NORMAL";
        case AnalogSensorEvent::UNDER_VALUE: return "UNDER";
        case AnalogSensorEvent::OVER_VALUE: return "OVER";
        case AnalogSensorEvent::ANALOG_SENSOR_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

class SubprocessSensor : public InputHandler
{
public:
    using EventCallback = std::function<void(AnalogSensorEvent)>;

    SubprocessSensor(EventCallback callback)
        : _callback(callback)
    {
        // Setup pipe for IPC: [0] is read, [1] is write
        if (pipe(_pipe_fds) == -1) {
            perror("pipe");
            throw std::runtime_error("Pipe creation failed");
        }
    }

    ~SubprocessSensor()
    {
        // Cleanup: kill child and close remaining fds
        if (_pid > 0) {
            kill(_pid, SIGTERM);
            waitpid(_pid, nullptr, 0);
        }
        if (_pipe_fds[0] != -1) close(_pipe_fds[0]);
        if (_pipe_fds[1] != -1) close(_pipe_fds[1]);
    }

    void start()
    {
        _pid = fork();
        if (_pid == -1) {
            perror("fork");
            throw std::runtime_error("Fork failed");
        }

        if (_pid == 0) {
            // Child: close read end, run logic, then exit
            close(_pipe_fds[0]);
            _pipe_fds[0] = -1;
            
            run_child_logic();
            
            close(_pipe_fds[1]);
            _exit(0);
        } else {
            // Parent: close write end
            close(_pipe_fds[1]);
            _pipe_fds[1] = -1;
        }
    }

    int get_fd() const { return _pipe_fds[0]; }

    // Eventloop callback when data is ready on the pipe
    EventAction ready(int fd) override
    {
        if (fd == _pipe_fds[0]) {
            AnalogSensorEvent ev;
            ssize_t n = read(fd, &ev, sizeof(ev));
            
            if (n == sizeof(ev)) {
                if (_callback) _callback(ev);
                return EventAction::Continue;
            } else {
                // Pipe closed or error -> stop the loop
                return EventAction::Quit;
            }
        }
        return EventAction::Continue;
    }

    void hookup(Eventloop& loop)
    {
        loop.register_input(get_fd(), this);
    }

private:
    void run_child_logic()
    {
        AnalogSensorMock bmp;
        bmp.set_value(1000.0f);

        float first = bmp.get_value();
        float under = first - 5.0f;
        float over = first + 5.0f;
        float hysteresis = 0.5f;
        int poll_ms = 1000;
        float step = 1.0f;

        AnalogSensorEventGenerator gen(&bmp, under, over, true, hysteresis);
        AnalogSensorEvent last = gen.get_event();
        
        // Push initial state to parent
        if (write(_pipe_fds[1], &last, sizeof(last)) == -1) return;
        std::printf("[Child] Initial event: %s pressure=%.2f\n", event_to_string(last), first);

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(poll_ms));

            // Mock sensor drift
            float val = bmp.get_value();
            if (val >= over + 2.0f) step = -1.0f;
            if (val <= under - 2.0f) step = 1.0f;
            bmp.set_value(val + step);

            float p = 0.0f;
            try {
                p = bmp.get_value();
            } catch (const std::exception& e) {
                std::fprintf(stderr, "[Child] Read error: %s\n", e.what());
                continue;
            }
            
            // Only send to parent if state changed
            AnalogSensorEvent ev = gen.get_event();
            if (ev != last) {
                std::printf("[Child] Change: %s -> %s @ %.2f\n", event_to_string(last), event_to_string(ev), p);
                if (write(_pipe_fds[1], &ev, sizeof(ev)) == -1) break;
                last = ev;
            }
        }
    }

    int _pipe_fds[2] = {-1, -1};
    pid_t _pid = -1;
    EventCallback _callback;
};

int main(int argc, char** argv)
{
    try {
        Eventloop loop;

        // Init sensor with a simple log callback
        SubprocessSensor sensor([](AnalogSensorEvent ev) {
            std::cout << "[Parent] Got event: " << event_to_string(ev) << std::endl;
        });

        sensor.start();
        sensor.hookup(loop);

        std::cout << "[Parent] Loop running..." << std::endl;
        loop.run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}