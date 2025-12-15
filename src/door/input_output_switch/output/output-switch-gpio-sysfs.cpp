#include "output-switch-gpio-sysfs.h"
#include <fstream>
#include <string>
#include <unistd.h> // for usleep
#include "small_file_handling.h"


OutputSwitchGPIOSysfs::OutputSwitchGPIOSysfs(unsigned int line_number)
    : _line_number(line_number)
{
    // Convert line number to string for sysfs paths.
    const std::string gpio_str = std::to_string(_line_number);
    const std::string gpio_path = "/sys/class/gpio/gpio" + gpio_str;

    if (dir_exists(gpio_path+"/direction") == false) {
        // Export the GPIO line if not already done.
        write_string_to_file("/sys/class/gpio/export", gpio_str);

        // A small delay might be needed for the sysfs directory to be created.
        usleep(100000); // 100ms
    }

    write_string_to_file(gpio_path + "/direction", "out");
}

OutputSwitchGPIOSysfs::~OutputSwitchGPIOSysfs()
{
    write_string_to_file("/sys/class/gpio/unexport", std::to_string(_line_number));
}

void OutputSwitchGPIOSysfs::set_state(State state)
{
    const std::string value = (state == State::OUTPUT_HIGH) ? "1" : "0";
    write_string_to_file("/sys/class/gpio/gpio" + std::to_string(_line_number) + "/value", value);
}
