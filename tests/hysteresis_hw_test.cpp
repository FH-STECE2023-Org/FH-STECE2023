#include <cstdio>
#include <door/analog_stuff/sensor/pressure-sensor-bmp280.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>
#include <door/utilities/i2c-real.h>  // Für I2CReal
#include <thread>
#include <chrono>
#include <memory>  // Für std::unique_ptr

int main(int argc, char** argv)
{
    using namespace std::chrono_literals;

    // Erstelle ein I2CReal-Objekt für den BMP280-Sensor
    std::unique_ptr<I2CReal> i2c_real = std::make_unique<I2CReal>("/dev/i2c-1", 0x76);

    // Erstelle den BMP280-Sensor mit dem I2CReal-Objekt
    BMP280 bmp(i2c_real.get());

    // Falls keine Schwellenwerte angegeben wurden, berechne Standardwerte um den ersten Messwert
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
