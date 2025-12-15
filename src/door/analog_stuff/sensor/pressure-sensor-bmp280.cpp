#include "pressure-sensor-bmp280.h"
#include <thread>
#include <chrono>
#include <stdexcept>

/*
 * BMP280 register addresses
 */
#define BMP280_REG_CONTROL        0xF4
#define BMP280_REG_CONFIG         0xF5
#define BMP280_REG_PRESSURE_MSB   0xF7
#define BMP280_REG_CALIB          0x88

/*
 * Constructor
 * -----------
 * Initializes the BMP280 sensor:
 * 1. Opens I2C connection via I2C utility
 * 2. Configures the sensor (oversampling, mode)
 * 3. Reads calibration data from the sensor
 */
BMP280::BMP280(const std::string& device, uint8_t address)
{
    // Create I2C connection to BMP280
    i2c_ = std::make_unique<I2C>(device, address);

    // Configure control register:
    //  - Temperature and Pressure oversampling x1
    //  - Normal mode
    uint8_t ctrl[2] = {BMP280_REG_CONTROL, 0x27};
    i2c_->write(ctrl, 2);

    // Configure config register:
    //  - Standby 0.5ms
    //  - Filter off
    uint8_t cfg[2] = {BMP280_REG_CONFIG, 0x00};
    i2c_->write(cfg, 2);

    // Wait 100ms for the sensor to stabilize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Read calibration data
    i2c_->write_reg(BMP280_REG_CALIB); // Set pointer to calibration start
    uint8_t calib[24];
    if (i2c_->read(calib, 24) != 24) {
        throw std::runtime_error("Failed to read BMP280 calibration data");
    }

    // Unpack calibration coefficients according to datasheet
    _dig_T1 = (calib[1] << 8) | calib[0];
    _dig_T2 = (calib[3] << 8) | calib[2];
    _dig_T3 = (calib[5] << 8) | calib[4];
    _dig_P1 = (calib[7] << 8) | calib[6];
    _dig_P2 = (calib[9] << 8) | calib[8];
    _dig_P3 = (calib[11] << 8) | calib[10];
    _dig_P4 = (calib[13] << 8) | calib[12];
    _dig_P5 = (calib[15] << 8) | calib[14];
    _dig_P6 = (calib[17] << 8) | calib[16];
    _dig_P7 = (calib[19] << 8) | calib[18];
    _dig_P8 = (calib[21] << 8) | calib[20];
    _dig_P9 = (calib[23] << 8) | calib[22];
}

/*
 * get_value()
 * -----------
 * Reads raw pressure (and temperature for compensation) from the BMP280
 * Applies Bosch datasheet compensation formulas
 * Returns pressure in hPa
 */
float BMP280::get_value() const
{
    // Set pointer to pressure MSB register
    i2c_->write_reg(BMP280_REG_PRESSURE_MSB);

    // Read 6 bytes: pressure MSB, LSB, XLSB, temperature MSB, LSB, XLSB
    uint8_t raw_data[6];
    if (i2c_->read(raw_data, 6) != 6) {
        throw std::runtime_error("Failed to read BMP280 raw pressure and temperature");
    }

    // Combine bytes to 20-bit raw values
    int32_t raw_pressure = (raw_data[0] << 12) | (raw_data[1] << 4) | (raw_data[2] >> 4);
    int32_t raw_temp     = (raw_data[3] << 12) | (raw_data[4] << 4) | (raw_data[5] >> 4);

    // Temperature compensation (t_fine) according to datasheet
    int32_t var1 = (((raw_temp >> 3) - ((int32_t)_dig_T1 << 1)) * ((int32_t)_dig_T2)) >> 11;
    int32_t var2 = (((((raw_temp >> 4) - ((int32_t)_dig_T1)) *
                      ((raw_temp >> 4) - ((int32_t)_dig_T1))) >> 12) * ((int32_t)_dig_T3)) >> 14;
    int32_t t_fine = var1 + var2;

    // Pressure compensation according to datasheet
    int64_t p_var1, p_var2, p;
    p_var1 = ((int64_t)t_fine) - 128000;
    p_var2 = p_var1 * p_var1 * (int64_t)_dig_P6;
    p_var2 = p_var2 + ((p_var1 * (int64_t)_dig_P5) << 17);
    p_var2 = p_var2 + (((int64_t)_dig_P4) << 35);
    p_var1 = ((p_var1 * p_var1 * (int64_t)_dig_P3) >> 8) + ((p_var1 * (int64_t)_dig_P2) << 12);
    p_var1 = (((((int64_t)1) << 47) + p_var1)) * ((int64_t)_dig_P1) >> 33;

    double pressure = 0;
    if (p_var1 != 0) {
        p = 1048576 - raw_pressure;
        p = (((p << 31) - p_var2) * 3125) / p_var1;
        p_var1 = (((int64_t)_dig_P9) * (p >> 13) * (p >> 13)) >> 25;
        p_var2 = (((int64_t)_dig_P8) * p) >> 19;
        p = ((p + p_var1 + p_var2) >> 8) + (((int64_t)_dig_P7) << 4);
        pressure = (double)p / 256.0 / 100.0; // Convert to hPa
    }

    return static_cast<float>(pressure);
}
