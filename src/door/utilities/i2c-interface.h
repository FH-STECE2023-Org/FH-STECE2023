#pragma once
#include <cstdint>
#include <string>

class I2CInterface {
public:
    virtual ~I2CInterface() = default;
    virtual ssize_t write(const void* buffer, size_t size) const = 0;
    virtual ssize_t read(void* buffer, size_t size) const = 0;
    virtual void write_reg(uint8_t reg) const = 0;
};
