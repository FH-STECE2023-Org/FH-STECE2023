#pragma once

#include "analog-sensor.h"
#include "utilities/i2c.h"
#include <memory>


/*
 * ADS1115 ADC
 * -----------
 * Reads analog voltage via I2C and returns it as a float.
 */
class Ads1115 : public AnalogSensor
{
public:
    Ads1115(const std::string& device_path, uint8_t i2c_address);

    float get_value() const override;

private:
    // Owns the I2C connection to the ADS1115
    std::unique_ptr<I2C> i2c_;

    // Converts raw ADC counts to volts
    float voltage_multiplier_;
};
