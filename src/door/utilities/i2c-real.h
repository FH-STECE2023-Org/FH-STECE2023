#pragma once
#include "i2c-interface.h"
#include <string>

class I2CReal : public I2CInterface {
public:
    I2CReal(const std::string& device, uint8_t address);
    ~I2CReal() override;
    ssize_t write(const void* buffer, size_t size) const override;
    ssize_t read(void* buffer, size_t size) const override;
    void write_reg(uint8_t reg) const override;

private:
    int fd_ = -1;
};
