#include <cstdio>
#include <door/analog_stuff/sensor/pressure-sensor-bmp280.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>
#include <thread>
#include <chrono>

int main(int argc, char** argv)
{
    using namespace std::chrono_literals;

    BMP280 bmp;

    // If thresholds were not provided, compute defaults around the first reading
    float first = bmp.get_value();
   
    float under = first - 5.0f;
    float over = first + 5.0f;
    float hysteresis = 0.5f;
    int poll_ms = 1000;

    AnalogSensorEventGenerator gen(&bmp, under, over, true, hysteresis);

    AnalogSensorEvent last = gen.get_event();
        {
            std::printf("Initial event: %d pressure=%.2f\n", static_cast<int>(last), bmp.get_value());
        }

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(poll_ms));
        float p = 0.0f;
        try {
            p = bmp.get_value();
            } catch (const std::exception& e) {
                std::fprintf(stderr, "Read error: %s\n", e.what());
                continue;
            }
        AnalogSensorEvent ev = gen.get_event();
        if (ev != last) {
        std::printf("Event changed: %d -> %d pressure=%.2f\n", static_cast<int>(last), static_cast<int>(ev), p);
            last = ev;
        }
    }

    return 0;
}
