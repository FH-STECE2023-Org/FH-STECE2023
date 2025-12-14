#pragma once

#include <cstdint>
#include <string>
#include <unistd.h>

/*
 * I2C
 * ---
 * This class represents a single I2C device on a bus.
 *
 * Responsibilities:
 *  - Open /dev/i2c-X
 *  - Select the slave address
 *  - Provide read/write access
 *  - Close the file descriptor automatically (RAII)
 *
 * What it does NOT do:
 *  - Sensor-specific logic
 *  - Register interpretation
 */

class I2C
{
public:
    /*
     * Constructor
     *  - device: e.g. "/dev/i2c-1"
     *  - address: 7-bit I2C address (e.g. 0x48, 0x76)
     */
    I2C(const std::string& device, uint8_t address);

    ~I2C();

    /*
     * Write raw bytes to the I2C device
     */
    ssize_t write(const void* buffer, size_t size) const;

    /*
     * Read raw bytes from the I2C device
     */
    ssize_t read(void* buffer, size_t size) const;

    /*
     * Common helper:
     *  - Write a single register address
     *  - Very common pattern for I2C sensors
     */
    void write_reg(uint8_t reg) const;

private:
    int fd_ = -1; // Linux file descriptor for the I2C device
};