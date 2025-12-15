#include "i2c.h"

#include <fcntl.h>
#include <stdexcept>
#include <sys/ioctl.h>

#ifdef __linux__
#include <linux/i2c-dev.h>
#else
#define I2C_SLAVE 0x0703
#endif

/*
 * Open the I2C device and select the slave address
 */
I2C::I2C(const std::string& device, uint8_t address)
{
    // Open /dev/i2c-X
    fd_ = open(device.c_str(), O_RDWR);
    if (fd_ < 0) {
        throw std::runtime_error("Failed to open I2C device: " + device);
    }

    // Tell the kernel which slave address we want to talk to
    if (ioctl(fd_, I2C_SLAVE, address) < 0) {
        close(fd_);
        throw std::runtime_error("Failed to set I2C slave address");
    }
}

/*
 * Destructor automatically releases the OS resource
 */
I2C::~I2C()
{
    if (fd_ >= 0) {
        close(fd_);
    }
}

/*
 * Write bytes to the device
 */
ssize_t I2C::write(const void* buffer, size_t size) const
{
    return ::write(fd_, buffer, size);
}

/*
 * Read bytes from the device
 */
ssize_t I2C::read(void* buffer, size_t size) const
{
    return ::read(fd_, buffer, size);
}

/*
 * Write a single register address
 * This is typically done before reading sensor data
 */
void I2C::write_reg(uint8_t reg) const
{
    if (write(&reg, 1) != 1) {
        throw std::runtime_error("Failed to write I2C register");
    }
}