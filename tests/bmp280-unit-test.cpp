#include <gtest/gtest.h>
#include <door/utilities/i2c-mock.h>
#include <door/analog_stuff/sensor/pressure-sensor-bmp280.h>

// Helpers
static std::vector<uint8_t> valid_calibration_data() {
    return {
        1,0,2,0,3,0,4,0,
        5,0,6,0,7,0,8,0,
        9,0,10,0,11,0,12,0
    };
}

static std::vector<uint8_t> valid_measurement_data() {
    return {0x80,0x00,0x00,0x80,0x00,0x00};
}

// ---------------------------- Tests -----------------------------

TEST(BMP280MockTest, FirstMeasurementFailsOnZeroData) {
    I2CMock mock;
    mock.push_read_response(valid_calibration_data());
    mock.push_read_response({0,0,0,0,0,0});

    BMP280 sensor(&mock);

    EXPECT_THROW(sensor.get_value(), std::runtime_error);
}

TEST(BMP280MockTest, ThirdMeasurementFailsOnZeroData) {
    I2CMock mock;
    mock.push_read_response(valid_calibration_data());
    mock.push_read_response(valid_measurement_data()); // 1st read
    mock.push_read_response(valid_measurement_data()); // 2nd read
    mock.push_read_response({0,0,0,0,0,0});            // 3rd read

    BMP280 sensor(&mock);

    EXPECT_NO_THROW(sensor.get_value()); // 1st
    EXPECT_NO_THROW(sensor.get_value()); // 2nd
    EXPECT_THROW(sensor.get_value(), std::runtime_error); // 3rd
}

// Optional: more tests can be added
