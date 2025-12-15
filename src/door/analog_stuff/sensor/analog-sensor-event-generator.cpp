#include "analog-sensor-event-generator.h"

AnalogSensorEventGenerator::AnalogSensorEventGenerator(AnalogSensor* sensor, 
                                                           float under_value, 
                                                           float over_value,
                                                           bool hysteresis_enabled,
                                                           float hysteresis_value)
{
    this->_sensor = sensor;
    this->_under_value = under_value;
    this->_over_value = over_value;

    this->_hysteresis_enabled = hysteresis_enabled;
    this->_hysteresis_value = hysteresis_value;

    this->_initialized = false;
    this->_last_event = AnalogSensorEvent::NORMAL_VALUE;
}

void AnalogSensorEventGenerator::enable_hysteresis(bool enable)
{
    this->_hysteresis_enabled = enable;
}

void AnalogSensorEventGenerator::set_hysteresis_value(float value)
{
    this->_hysteresis_value = value;
}

AnalogSensorEvent AnalogSensorEventGenerator::get_event()
{
    float pressure = _sensor->get_value();

    // Immediate sensor error (no hysteresis)
    if (pressure < 0.0f) {
        _last_event = AnalogSensorEvent::ANALOG_SENSOR_ERROR;
        _initialized = true;
        return _last_event;
    }

    // If hysteresis is disabled or not yet initialized: behave as before and initialize state
    if (!_hysteresis_enabled || !_initialized) {
        if (pressure < _under_value && pressure >= 0.0f) {
            _last_event = AnalogSensorEvent::UNDER_VALUE;
        } else if (pressure > _over_value) {
            _last_event = AnalogSensorEvent::OVER_VALUE;
        } else if (pressure >= 0.0f) {
            _last_event = AnalogSensorEvent::NORMAL_VALUE;
        } else {
            _last_event = AnalogSensorEvent::ANALOG_SENSOR_ERROR;
        }
        _initialized = true;
        return _last_event;
    }

    // Hysteresis enabled and initialized: use margin logic per state
    float h = _hysteresis_value;

    switch (_last_event) {
        case AnalogSensorEvent::OVER_VALUE:
            // stay in OVER unless we have fallen below (over - h)
            if (pressure <= (_over_value - h)) {
                // now evaluate whether under or normal
                if (pressure < (_under_value - h)) {
                    _last_event = AnalogSensorEvent::UNDER_VALUE;
                } else if (pressure > (_over_value + h)) {
                    _last_event = AnalogSensorEvent::OVER_VALUE;
                } else {
                    _last_event = AnalogSensorEvent::NORMAL_VALUE;
                }
            } else {
                // remain OVER
                _last_event = AnalogSensorEvent::OVER_VALUE;
            }
            break;

        case AnalogSensorEvent::UNDER_VALUE:
            // stay in UNDER unless we have risen above (under + h)
            if (pressure >= (_under_value + h)) {
                if (pressure > (_over_value + h)) {
                    _last_event = AnalogSensorEvent::OVER_VALUE;
                } else if (pressure < (_under_value - h)) {
                    _last_event = AnalogSensorEvent::UNDER_VALUE;
                } else {
                    _last_event = AnalogSensorEvent::NORMAL_VALUE;
                }
            } else {
                // remain UNDER
                _last_event = AnalogSensorEvent::UNDER_VALUE;
            }
            break;

        case AnalogSensorEvent::NORMAL_VALUE:
        default:
            // require crossing thresholds with hysteresis to change state
            if (pressure > (_over_value + h)) {
                _last_event = AnalogSensorEvent::OVER_VALUE;
            } else if (pressure < (_under_value - h)) {
                _last_event = AnalogSensorEvent::UNDER_VALUE;
            } else {
                _last_event = AnalogSensorEvent::NORMAL_VALUE;
            }
            break;
    }

    return _last_event;
}