#pragma once

#include <cstdint>
#include <string>
#include <unistd.h>

class I2C
{
public:
    I2C(const std::string& device, uint8_t address);

    ~I2C();

    ssize_t write(const void* buffer, size_t size) const;

    ssize_t read(void* buffer, size_t size) const;

    void write_reg(uint8_t reg) const;

private:
    int fd_ = -1;
};
