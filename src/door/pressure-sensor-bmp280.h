#pragma once

#include "analog-sensor.h"
#include "utilities/i2c.h"
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
     *  - device: I2C device path (e.g., "/dev/i2c-1")
     *  - address: I2C address of the sensor (default 0x76)
     */
    BMP280(const std::string& device = "/dev/i2c-1", uint8_t address = 0x76);

    /*
     * Destructor
     *  - defaulted here in the header
     *  - unique_ptr handles cleanup of the I2C connection automatically
     */
    ~BMP280() override = default;

    /*
     * Read compensated pressure in hPa
     */
    float get_value() const override;

private:
    // I2C connection to the sensor
    std::unique_ptr<I2C> i2c_;

    // Calibration coefficients from sensor
    uint16_t _dig_T1;
    int16_t  _dig_T2, _dig_T3;
    uint16_t _dig_P1;
    int16_t  _dig_P2, _dig_P3, _dig_P4, _dig_P5;
    int16_t  _dig_P6, _dig_P7, _dig_P8, _dig_P9;
};
