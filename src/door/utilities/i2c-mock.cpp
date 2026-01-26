#include "i2c-mock.h"
#include <cstring>

ssize_t I2CMock::write(const void*, size_t size) const {
    return size;
}

ssize_t I2CMock::read(void* buffer, size_t size) const {
    if (read_queue_.empty()) {
        throw std::runtime_error("Unexpected I2C read");
    }

    auto data = read_queue_.front();
    read_queue_.erase(read_queue_.begin());

    if (data.size() != size) {
        throw std::runtime_error("I2C read size mismatch");
    }

    std::memcpy(buffer, data.data(), size);
    return size;
}

void I2CMock::write_reg(uint8_t) const {
    // no-op
}

void I2CMock::push_read_response(const std::vector<uint8_t>& data) {
    read_queue_.push_back(data);
}

