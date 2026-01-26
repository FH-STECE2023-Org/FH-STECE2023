#include "i2c-real.h"
#include <fcntl.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <unistd.h>

#ifdef __linux__
#include <linux/i2c-dev.h>
#else
#define I2C_SLAVE 0x0703
#endif

I2CReal::I2CReal(const std::string& device, uint8_t address) {
    fd_ = open(device.c_str(), O_RDWR);
    if (fd_ < 0) {
        throw std::runtime_error("Failed to open I2C device: " + device);
    }

    if (ioctl(fd_, I2C_SLAVE, address) < 0) {
        close(fd_);
        throw std::runtime_error("Failed to set I2C slave address");
    }
}

I2CReal::~I2CReal() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

ssize_t I2CReal::write(const void* buffer, size_t size) const {
    return ::write(fd_, buffer, size);
}

ssize_t I2CReal::read(void* buffer, size_t size) const {
    return ::read(fd_, buffer, size);
}

void I2CReal::write_reg(uint8_t reg) const {
    if (write(&reg, 1) != 1) {
        throw std::runtime_error("Failed to write I2C register");
    }
}
