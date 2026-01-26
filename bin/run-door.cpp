#include <door/statemachine/door.h>
#include <door/statemachine/structs.h>

#include <door/statemachine/inputs.h>
#include <door/statemachine/outputs.h>

#include <door/input_output_switch/input/input-switch.h>
#include <door/input_output_switch/input/input-switch-mock.h>
#include <door/input_output_switch/input/input-switch-gpio-sysfs.h>
#include <door/input_output_switch/output/output-switch.h>

#include <door/analog_stuff/sensor/analog-sensor.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>
#include <door/analog_stuff/sensor/analog-sensor-mock.h>
#include <door/analog_stuff/sensor/pressure-sensor-bmp280.h>
#include <door/utilities/i2c-real.h>

#include <door/motor/motor-mock.h>

#include <door/utilities/timespec.h>

#include <string>
#include <iostream>
#include <signal.h>
#include <memory>

#include <door/utilities/eventloop.h>
#include <door/utilities/periodic-timer.h>
#include <door/utilities/one-shot-timer.h>
#include <door/utilities/graceful-term.h>

// quit flag with atomic type
static volatile sig_atomic_t quit = 0;

// handler function to set quit flag
static void handler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
        quit = 1;
}

int main(int argc, char** argv)
{
    int test = 0;
    int real = 0;

    if (argc > 2)
    {
        std::cout << "Error: Too many arguments!" << std::endl;
        std::cout << "Usage: ./run-door [--test/--real]" << std::endl;
        return 1;
    }

    if (argc == 2)
    {
        std::string flag = argv[1];
        if (flag == "--test") test = 1;
        else if (flag == "--real") real = 1;
        else
        {
            std::cout << "Error: Invalid argument!" << std::endl;
            std::cout << "Usage: ./run-door [--test/--real]" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cout << "Error: Missing argument!" << std::endl;
        std::cout << "Usage: ./run-door [--test/--real]" << std::endl;
        return 1;
    }

    // event handler for SIGTERM and SIGINT
    struct sigaction sa = { 0 };
    sa.sa_handler = handler;

    if (sigaction(SIGTERM, &sa, nullptr) == -1)
    {
        perror("sigaction(SIGTERM)");
        return 1;
    }
    if (sigaction(SIGINT, &sa, nullptr) == -1)
    {
        perror("sigaction(SIGINT)");
        return 1;
    }

    Door door;

    // Shared ownership for all resources
    std::shared_ptr<InputSwitch> button_outside;
    std::shared_ptr<InputSwitch> button_inside;
    std::shared_ptr<InputSwitch> lightbarrier_closed;
    std::shared_ptr<InputSwitch> lightbarrier_open;
    std::shared_ptr<AnalogSensor> pressureSensor;
    std::shared_ptr<Motor> motor;

    // Keep I2C alive as long as the sensor lives
    std::shared_ptr<I2CReal> i2c;

    if (test)
    {
        std::cout << "Info: Test run, only using mock sensors." << std::endl;

        button_outside = std::make_shared<InputSwitchMock>(InputSwitch::State::INPUT_LOW);
        button_inside  = std::make_shared<InputSwitchMock>(InputSwitch::State::INPUT_LOW);

        lightbarrier_closed = std::make_shared<InputSwitchMock>(InputSwitch::State::INPUT_LOW);
        lightbarrier_open   = std::make_shared<InputSwitchMock>(InputSwitch::State::INPUT_HIGH);

        pressureSensor = std::make_shared<AnalogSensorMock>();
        motor = std::make_shared<MotorMock>(Motor::Direction::IDLE);
    }
    else if (real)
    {
        std::cout << "Info: Real run, using real sensors." << std::endl;

        button_outside = std::make_shared<InputSwitchGPIOSysfs>(17);
        button_inside  = std::make_shared<InputSwitchGPIOSysfs>(27);
        lightbarrier_closed = std::make_shared<InputSwitchGPIOSysfs>(22);
        lightbarrier_open   = std::make_shared<InputSwitchGPIOSysfs>(23);

        i2c = std::make_shared<I2CReal>("/dev/i2c-1", 0x76);
        pressureSensor = std::make_shared<BMP280>(i2c.get());

        // motor = std::make_shared<MotorStepper>(...);
    }

    // Event generator uses the sensor, lifetime guaranteed by shared_ptr
    AnalogSensorEventGenerator pressureSensorEG(pressureSensor.get());

    TimeSpec time;

    Inputs inputs(
        button_outside.get(),
        button_inside.get(),
        lightbarrier_closed.get(),
        lightbarrier_open.get(),
        &pressureSensorEG,
        time);

    Outputs outputs(motor.get());

    input_t in = inputs.get_inputs();
    output_t out = door.init(in);
    outputs.set_outputs(out);

    GracefulTerminator terminator;

    // 1 ms periodic timer
    TimeSpec set_periodic(0, 1000000);
    PeriodicTimer periodic_timer(set_periodic,
        [&inputs, &outputs, &door]()
        {
            events_t ev = inputs.get_events();
            output_t out = door.cyclic(ev);
            outputs.set_outputs(out);
        });

    // 1 s one-shot timer
    TimeSpec set_oneshot(1, 0);
    OneShotTimer oneshot_timer(set_oneshot,
        []()
        {
            std::cout << "One Shot expired" << std::endl;
        });

    Eventloop loop;
    terminator.hookup(loop);
    periodic_timer.hookup(loop);
    oneshot_timer.hookup(loop);

    loop.run();

    // No manual delete needed (shared_ptr)

    if (oneshot_timer.isrunning())
    {
        std::cout << "\nOne Shot Timer still running, deppert" << std::endl;
        return 1;
    }

    std::cout << "\nOh, I need to go, someone is calling me..." << std::endl;
    std::cout << "Bye, see you soon :)" << std::endl;
    std::cout << "I'll miss you <3" << std::endl;
    std::cout << "  -- yours, Depperte Door" << std::endl << std::endl;

    return 0;
}
