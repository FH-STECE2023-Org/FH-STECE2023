#include <gtest/gtest.h>
#include <door/analog_stuff/adc/adc-ads1115.h>
#include <memory>
#include <stdexcept>

TEST(Ads1115IntegrationTest, MeasuresVoltageWithinTolerance) {
    
    const float expected_voltage = 1.6f;
    const float tolerance = 0.1f;

    try {
        Ads1115 adc("/dev/i2c-1", 0x48);

	// sollte ca. 1.6V sein, wenn Joystick angeschlossen und nicht bewegt wird 
        float measured_voltage = adc.get_value();

        EXPECT_NEAR(measured_voltage, expected_voltage, tolerance)
            << "Die gemessene Spannung von " << measured_voltage
            << "V weicht zu stark vom Erwartungswert " << expected_voltage << "V ab.";

    } catch (const std::runtime_error& e) {
        FAIL() << "Test konnte nicht ausgeführt werden. Fehler: " << e.what();
    }
}
