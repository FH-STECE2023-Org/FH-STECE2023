#pragma once

#include "analog-sensor.h"
#include <door/utilities/i2c-interface.h>
#include <memory>

/*
 * BMP280 pressure sensor
 * ----------------------
 * Reads pressure via I2C and applies Bosch compensation formulas.
 */
class BMP280 : public AnalogSensor
{
public:
    /*
     * Constructor
     *  - i2c: Pointer to an I2CInterface object (ownership remains with the caller)
     */
    BMP280(I2CInterface* i2c);

    /*
     * Destructor
     *  - defaulted here in the header
     */
    ~BMP280() override = default;

    /*
     * Read compensated pressure in hPa
     */
    float get_value() const override;

private:
    // I2C connection to the sensor
    I2CInterface* i2c_;

    // Calibration coefficients from sensor
    uint16_t _dig_T1;
    int16_t  _dig_T2, _dig_T3;
    uint16_t _dig_P1;
    int16_t  _dig_P2, _dig_P3, _dig_P4, _dig_P5;
    int16_t  _dig_P6, _dig_P7, _dig_P8, _dig_P9;
};
