#include <door/input_output_switch/input/input-edge-event.h>
#include <door/utilities/eventloop.h>

#include <chrono>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>

int main()
{
    try {
        // Adjust for your hardware
        constexpr int GPIO_PIN = 17+512;
        constexpr int MIN_EVENTS = 5;

        // edge: "rising", "falling", "both"
        const std::string edge = "rising";

        std::cout << "Waiting for GPIO edge on pin " << GPIO_PIN << " (" << edge << ")..." << std::endl;

        // Keep the event loop running; we'll count events in main()
        InputEdgeEvent gpio_event(GPIO_PIN, edge, EventAction::Continue);
        Eventloop loop;
        gpio_event.hookup(loop);

        for (int count = 1; count <= MIN_EVENTS; ++count) {
            loop.run_one();

            const auto now = std::chrono::system_clock::now();
            const std::time_t t = std::chrono::system_clock::to_time_t(now);

            std::tm tm{};
#if defined(_WIN32)
            localtime_s(&tm, &t);
#else
            localtime_r(&t, &tm);
#endif

            std::ostringstream ts;
            ts << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

            std::cout << ts.str() << " event received " << count << std::endl;
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
