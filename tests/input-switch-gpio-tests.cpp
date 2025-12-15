#include <door/input_output_switch/input/input-switch-gpio-sysfs.h>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    try {
        unsigned int input_line = 529;

        // Instantiate the inpuswitch 
        InputSwitchGPIOSysfs inputswitch(input_line);

        std::cout << "Reading GPIO line " << input_line << " on " << std::endl;

        // Read and print state 10 times (1s interval)
        for (int i = 0; ; ++i) {
            InputSwitch::State state = inputswitch.get_state();
            if (state == InputSwitch::State::INPUT_HIGH)
                std::cout << "State: INPUT HIGH\n";
            else
                std::cout << "State: INPUT LOW\n";

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::system_error& e) {
        std::cerr << "System error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}
