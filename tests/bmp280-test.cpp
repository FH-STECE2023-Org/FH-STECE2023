#include <iostream>
#include <stdexcept>
#include <door/analog_stuff/sensor/pressure-sensor-bmp280.h>
#include <door/utilities/i2c-real.h>

// Dies ist ein einfaches, eigenständiges Programm mit einer main()-Funktion.
int main()
{
    try
    {
        // Erzeuge das Sensor-Objekt. Die Initialisierung passiert im Konstruktor.
        I2CReal* i2c = new I2CReal("/dev/i2c-1", 0x76);
        BMP280* pressureSensor = new BMP280(i2c);


        // Lies den Druck aus und gib ihn auf der Konsole aus.
        float pressure = pressureSensor->get_value();
        std::cout << "Pressure: " << pressure << " hPa" << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        // Fange Fehler ab, falls der Sensor nicht gefunden oder gelesen werden kann.
        std::cerr << "Error: " << e.what() << std::endl;
        return 1; // Beende das Programm mit einem Fehlercode.
    }

    return 0; // Erfolgreiches Ende.
}
