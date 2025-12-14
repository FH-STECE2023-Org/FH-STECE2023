#include "adc-ads1115.h"
#include <unistd.h>
#include <stdexcept>

// ADS1115 register addresses
constexpr uint8_t ADS1115_REG_CONVERSION = 0x00;
constexpr uint8_t ADS1115_REG_CONFIG     = 0x01;

Ads1115::Ads1115(const std::string& device_path, uint8_t i2c_address)
{
    // Conversion factor for gain ±4.096V
    voltage_multiplier_ = 4.096f / 32767.0f;

    // Create the I2C connection
    i2c_ = std::make_unique<I2C>(device_path, i2c_address);

    // Configure ADS1115 for continuous conversion
    uint8_t config[3] = {
        ADS1115_REG_CONFIG,
        0xC2, // Continuous mode, AIN0, gain ±4.096V
        0x83  // 128 samples per second
    };

    if (i2c_->write(config, 3) != 3) {
        throw std::runtime_error("ADS1115 configuration failed");
    }

    // Wait until first conversion is ready
    usleep(8 * 1000);
}

float Ads1115::get_value() const
{
    // Tell ADS1115 we want the conversion register
    i2c_->write_reg(ADS1115_REG_CONVERSION);

    // Read 16-bit conversion result
    uint8_t data[2];
    if (i2c_->read(data, 2) != 2) {
        throw std::runtime_error("ADS1115 read failed");
    }

    // Combine MSB + LSB into signed value
    int16_t raw_adc = (data[0] << 8) | data[1];

    // Convert to volts
    return raw_adc * voltage_multiplier_;
}