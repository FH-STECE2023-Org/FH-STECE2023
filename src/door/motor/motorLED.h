#pragma once

#include "motor.h"
#include "output-switch.h" // Einbinden der neuen Abstraktion

class MotorLED : public Motor
{
public:
    // Der Konstruktor nimmt jetzt Referenzen auf zwei OutputSwitch-Objekte entgegen.
    MotorLED(OutputSwitch& forward_switch, OutputSwitch& backward_switch);

    // Der Destruktor ist trivial, da die Klasse keine Ressourcen mehr verwaltet.
    ~MotorLED() override = default;

    void forward() override;
    void backward() override;
    void stop() override;
    Direction get_direction() const override;

private:
    // Referenzen auf die abstrakten Schalter
    OutputSwitch& _forward_switch;
    OutputSwitch& _backward_switch;

    Direction _direction;
};
