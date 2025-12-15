#include "motorLED.h"
#include <cassert>

// Konstruktor
// Initialisiert die Member-Referenzen und setzt den Startzustand.
MotorLED::MotorLED(OutputSwitch& forward_switch, OutputSwitch& backward_switch)
    : _forward_switch(forward_switch), _backward_switch(backward_switch), _direction(Direction::IDLE)
{
    // Sicherstellen, dass der Motor initial gestoppt ist.
    stop();
}

// Funktionsdefinitionen
void MotorLED::forward()
{
    // Vorwärts: forward_switch AN, backward_switch AUS
    _forward_switch.set_state(OutputSwitch::State::OUTPUT_HIGH);
    _backward_switch.set_state(OutputSwitch::State::OUTPUT_LOW);
    _direction = Direction::FORWARD;
}

void MotorLED::backward()
{
    // Rückwärts: forward_switch AUS, backward_switch AN
    _forward_switch.set_state(OutputSwitch::State::OUTPUT_LOW);
    _backward_switch.set_state(OutputSwitch::State::OUTPUT_HIGH);
    _direction = Direction::BACKWARD;
}

void MotorLED::stop()
{
    // Stopp: Beide Schalter AUS
    _forward_switch.set_state(OutputSwitch::State::OUTPUT_LOW);
    _backward_switch.set_state(OutputSwitch::State::OUTPUT_LOW);
    _direction = Direction::IDLE;
}

Motor::Direction MotorLED::get_direction() const
{
    return _direction;
}