#include "motor-stepper.h"
#include <string>
#include <stdexcept>
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "output-switch.h"



//Konstruktor
MotorStepper::MotorStepper(const std::string& gpiodevice, OutputSwitch& line_enable, OutputSwitch& line_direction, std::string period_nanosec, std::string duty_nanosec)
    : _line_enable(line_enable), _line_direction(line_direction), _direction(Direction::IDLE), _period_nanosec(period_nanosec), _duty_nanosec(duty_nanosec){
    
    //MotorStepper::ensureExported();
    MotorStepper::stop();
}


//Dekonstruktor
MotorStepper::~MotorStepper(){

}

//Funktionsdefinitionen
void MotorStepper::forward(){
    
    _line_enable.set_state(OutputSwitch::State::OUTPUT_LOW);
    _line_direction.set_state(OutputSwitch::State::OUTPUT_LOW);

    _direction = Direction::FORWARD;

    writeData("/sys/class/pwm/pwmchip0/pwm0/period", _period_nanosec);
    writeData("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", _duty_nanosec);
    writeData("/sys/class/pwm/pwmchip0/pwm0/enable", "1");
}

void MotorStepper::backward(){

    _line_enable.set_state(OutputSwitch::State::OUTPUT_LOW);
    _line_direction.set_state(OutputSwitch::State::OUTPUT_HIGH);

    _direction = Direction::BACKWARD;

    writeData("/sys/class/pwm/pwmchip0/pwm0/period", _period_nanosec);
    writeData("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", _duty_nanosec);
    writeData("/sys/class/pwm/pwmchip0/pwm0/enable", "1");
}

void MotorStepper::stop(){

    _line_enable.set_state(OutputSwitch::State::OUTPUT_HIGH);
    _line_direction.set_state(OutputSwitch::State::OUTPUT_LOW);

    _direction = Direction::IDLE;

    writeData("/sys/class/pwm/pwmchip0/pwm0/enable", "0");
}

Motor::Direction MotorStepper::get_direction() const{

    return _direction;
}

// writeData überschreibt zu sendende Daten
int MotorStepper::writeData(std::string path, std::string value) {
    FILE *file = fopen(path.c_str(), "w");
    if (!file) {
        perror("fopen");
        return -1;
    }
    if (fputs(value.c_str(), file) == EOF) {
        perror("fputs");
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

/*
//stellt sicher das pwm0 exportiert ist
int MotorStepper::ensureExported() {
    struct stat st;
    if (stat("/sys/class/pwm/pwmchip0/pwm0", &st) != 0) {
        // Exportieren, falls pwm0 nicht vorhanden ist
        if (writeData("/sys/class/pwm/pwmchip0/export", "0") < 0) {
            return -1;
        }
    }
    return 0;
}
*/
