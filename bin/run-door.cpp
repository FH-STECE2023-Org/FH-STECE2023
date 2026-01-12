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

#include <door/motor/motor-mock.h>

#include <door/utilities/timespec.h>

#include <string>
#include <iostream>
#include <signal.h>

#include <door/utilities/eventloop.h>
#include <door/utilities/periodic-timer.h>
#include <door/utilities/graceful-term.h>

#include <door/utilities/timespec.h>

// quit flag with atomic type
static volatile sig_atomic_t quit = 0;

// hander function to set quit flag
static void handler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
        quit = 1;
}

int main(int argc, char** argv)
{
    // test flag
    int test = 0;
    int real = 0;


    // too many arguments
    if (argc > 2)
    {
        std::cout << "Error: Too many arguments!" << std::endl;
        std::cout << "Usage: ./run-door [--test/--real]" << std::endl;
        
        return 1;
    }

    // one additional argument
    if (argc == 2)
    {
        std::string flag = argv[1];
        if (flag == "--test")
        {
            test = 1;
        }
        else if(flag == "--real")
        {
            real = 1;
        }
        else
        {
            std::cout << "Error: Invalide argument!" << std::endl;
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

    int rv = sigaction(SIGTERM, &sa, nullptr);
    if (rv == -1)
    {
        perror("sigaction(SIGTERM)");
        return 1;
    }
    rv = sigaction(SIGINT, &sa, nullptr);
    if (rv == -1)
    {
        perror("sigaction(SIGINT)");
        return 1;
    }

    // create door
    Door door;

    InputSwitch* button_outside;
    InputSwitch* button_inside;
    InputSwitch* lightbarrier_closed;
    InputSwitch* lightbarrier_open;
    AnalogSensor* pressureSensor;
    Motor* motor;

    if (test)
    {
        // Mock sensors
        std::cout << "Info: Test run, only using mock sensors." << std::endl;

        // Buttons
        button_outside = new InputSwitchMock(InputSwitch::State::INPUT_LOW);
        button_inside = new InputSwitchMock(InputSwitch::State::INPUT_LOW);
        // Lightbarriers
        lightbarrier_closed = new InputSwitchMock(InputSwitch::State::INPUT_LOW);
        lightbarrier_open = new InputSwitchMock(InputSwitch::State::INPUT_HIGH);
        // Pressure sensor
        pressureSensor = new AnalogSensorMock();
        // Motor
        motor = new MotorMock(Motor::Direction::IDLE);
    }
    else if (real)
    {
        std::cout << "Info: Real run, using real sensors." << std::endl;
        // create sensors
        button_outside = new InputSwitchGPIOSysfs(17);
        button_inside = new InputSwitchGPIOSysfs(27);
        lightbarrier_closed  = new InputSwitchGPIOSysfs(22);
        lightbarrier_open  = new InputSwitchGPIOSysfs(23);

        // Pressure Sensor
        pressureSensor = new BMP280("/dev/i2c-1", 0x76); 

        //motor = new MotorStepper("/dev/gpiochip0", 26, 17, "2000000", "1000000");
    }

    // Pressure Sensor Event Generator
    AnalogSensorEventGenerator pressureSensorEG(pressureSensor);

    TimeSpec time;

    Inputs inputs(button_outside, button_inside, lightbarrier_closed, lightbarrier_open, &pressureSensorEG, time);
    Outputs outputs(motor);

    input_t in;
    output_t out;
    
    // get current inputs and create input struct
    in = inputs.get_inputs();

    // run door init and return output struct
    out = door.init(in);

    // set outputs
    outputs.set_outputs(out);

    //1ms time
    TimeSpec set_time(0, 1000000);

    //Eventloop
    Eventloop loop;

    PeriodicTimer timer_handler(set_time,
                                [&inputs, &outputs, &door]()
                                {
                                    events_t ev  = inputs.get_events();
                                    output_t out = door.cyclic(ev);
                                    outputs.set_outputs(out);
                                });
    GracefulTerminator terminator;
    terminator.hookup(loop);
    timer_handler.hookup(loop);
    loop.run();



    // cleanup before exit
    delete button_outside;
    delete button_inside;
    delete lightbarrier_closed;
    delete lightbarrier_open;
    delete pressureSensor;
    delete motor;

    // Bye message
    std::cout << std::endl;
    std::cout << "Oh, I need to go, someone is calling me..." << std::endl;
    std::cout << "Bye, see you soon :)" << std::endl;
    std::cout << "I'll miss you <3" << std::endl;
    std::cout << "  -- yours, Depperte Door" << std::endl << std::endl;

    return 0;
}
