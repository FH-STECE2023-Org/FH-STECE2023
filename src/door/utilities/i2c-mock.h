#pragma once
#include "i2c-interface.h"
#include <vector>
#include <cstdint>
#include <stdexcept>

class I2CMock : public I2CInterface {
public:
    ssize_t write(const void* buffer, size_t size) const override;
    ssize_t read(void* buffer, size_t size) const override;
    void write_reg(uint8_t reg) const override;

    // Queue read responses (each entry = one read call)
    void push_read_response(const std::vector<uint8_t>& data);

private:
    mutable std::vector<std::vector<uint8_t>> read_queue_;
};
    
