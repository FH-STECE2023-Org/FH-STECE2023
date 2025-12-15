#include <door/input_output_switch/input/input-switch-gpio-sysfs.h>
#include <door/input_output_switch/output/output-switch-gpio-sysfs.h>
#include <gtest/gtest.h>
#include <unistd.h>

int main() {
    try {

        std::cout << "Start Init." << std::endl;

        unsigned int input_line = 529;
        unsigned int output_line = 539;

        InputSwitchGPIOSysfs inputswitch(input_line);
        OutputSwitchGPIOSysfs outputswitch(output_line);

        std::cout << "Init done 8==D" << std::endl;

        // Detect HIGH from Output to Input
        outputswitch.set_state(OutputSwitch::State::OUTPUT_HIGH);
        usleep(50000);

        if(inputswitch.get_state() == InputSwitch::State::INPUT_HIGH)
            std::cout << "High detection success :)" << std::endl;
        else
        {
            std::cerr << "High detection error :(" << std::endl;
            std::cout << "High detection error :(" << std::endl;
        }

        // Detect LOW from Output to Input
        outputswitch.set_state(OutputSwitch::State::OUTPUT_LOW);
        usleep(50000);

        if(inputswitch.get_state() == InputSwitch::State::INPUT_LOW)
            std::cout << "Low detection success :)" << std::endl;
        else
        {
            std::cerr << "Low detection error :(" << std::endl;
            std::cout << "Low detection error :(" << std::endl;
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