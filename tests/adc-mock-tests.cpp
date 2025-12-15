#include <gtest/gtest.h>
#include <door/analog_stuff/sensor/analog-sensor-mock.h>

TEST(Adc_mock_suite, Adc_test)
{
    AnalogSensorMock adc;

    adc.set_value(3.3f);

    ASSERT_NEAR(adc.get_value(), 3.3f, 0.001);

};
