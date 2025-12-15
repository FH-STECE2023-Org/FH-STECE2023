// TEST either with motorLED or motorStepper (--stepper, --led), defaults to motorLED

#include <iostream>
#include <string>
#include <memory>      // Nicht mehr nötig für unique_ptr, aber lassen wir es
#include <unistd.h>    // Für sleep()
#include <cstdio>      // Für printf

#include <door/motor/motorLED.h>
#include <door/input_output_switch/output/output-switch-gpio-sysfs.h>
#include <door/motor/motor-stepper.h>
#include <door/motor/motor.h>

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s [--led|--stepper]\n", argv[0]);
        return 1; // Mit Fehler beenden
    }

    OutputSwitchGPIOSysfs forwardSwitch(20+512); 
    OutputSwitchGPIOSysfs backwardSwitch(21+512);
    
    // --- DEMONSTRATION DES MEMORY LEAKS ---
    // 1. Wir verwenden einen rohen Pointer (rohen Zeiger)
    Motor* motor;
    std::string motorType = std::string(argv[1]);

    if (motorType == "--led")
    {
        std::cout << "[INFO] Erstelle MotorLED..." << std::endl;
        // 2. Wir reservieren Speicher auf dem Heap mit 'new'
        motor = new MotorLED(forwardSwitch, backwardSwitch);
    }
    else if (motorType == "--stepper")
    {
        std::cout << "[WARN] Stepper noch nicht implementiert, verwende MotorLED." << std::endl;
        motor = new MotorLED(forwardSwitch, backwardSwitch);
    }
    else
    {
        std::cout << "[WARN] Unbekannter Motor-Typ. Verwende Standard: MotorLED." << std::endl;
        motor = new MotorLED(forwardSwitch, backwardSwitch);
    }

    // Tests ausführen (der -> Operator funktioniert gleich)
    std::cout << "[TEST] motor->forward()" << std::endl;
    motor->forward();
    sleep(2);

    std::cout << "[TEST] motor->backward()" << std::endl;
    motor->backward();
    sleep(2);

    std::cout << "[TEST] motor->stop()" << std::endl;
    motor->stop();
    
    std::cout << "[INFO] Test beendet." << std::endl;
    
    // 3. DAS SPEICHERLECK:
    // Der 'motor'-Pointer geht hier "out of scope" (verlässt die main-Funktion).
    // Da 'delete motor;' NICHT aufgerufen wird, wird der Speicher, 
    // der von 'new MotorLED(...)' belegt wurde, *niemals* freigegeben.
    // Das ist das Speicherleck.

    // delete motor; // <-- Diese Zeile fehlt absichtlich!
    
    return 0;
}
