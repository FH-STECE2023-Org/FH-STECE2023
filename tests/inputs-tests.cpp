#include <gtest/gtest.h>
#include <stdbool.h>

#include <door/door.h> 
#include <door/structs.h> 


#include <door/input_output_switch/input/inputs.h>
#include <door/input_output_switch/input/input-switch-mock.h>

#include <door/event-edge-detector.h>

#include <door/analog_stuff/sensor/analog-sensor-mock.h>
#include <door/analog_stuff/sensor/analog-sensor-event-generator.h>

TEST(input_suite, input_init)
{
    
    InputSwitchMock button_outside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock button_inside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock lightbarrier_closed(InputSwitch::State::INPUT_LOW);  // <-- door in "closed" position
    InputSwitchMock lightbarrier_open(InputSwitch::State::INPUT_HIGH);   // <-- door not in "opened" position
    
    AnalogSensorMock pressureSensor;
    AnalogSensorEventGenerator pressureSensorEG(&pressureSensor);

    // create Input struct
    TimeSpec time;
    Inputs inputs_set(&button_outside, &button_inside, &lightbarrier_closed, &lightbarrier_open, &pressureSensorEG, time);

    input_t inputs;
    events_t events;

    inputs = inputs_set.get_inputs();
    events = inputs_set.get_events();


    // check state and output
    ASSERT_EQ(inputs.button_inside, InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(inputs.button_inside, InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(inputs.sensor_closed, InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(inputs.sensor_open, InputSwitch::State::INPUT_HIGH);
}

TEST(input_suite, input_switch)
{
    
    InputSwitchMock button_outside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock button_inside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock lightbarrier_closed(InputSwitch::State::INPUT_LOW);  // <-- door in "closed" position
    InputSwitchMock lightbarrier_open(InputSwitch::State::INPUT_HIGH);   // <-- door not in "opened" position

    AnalogSensorMock pressureSensor;
    AnalogSensorEventGenerator pressureSensorEG(&pressureSensor);

    // create Input struct
    TimeSpec time;
    Inputs inputs_set(&button_outside, &button_inside, &lightbarrier_closed, &lightbarrier_open, &pressureSensorEG, time);

    input_t inputs;
    events_t events;

    inputs = inputs_set.get_inputs();
    events = inputs_set.get_events();


    // check state and output
    ASSERT_EQ(inputs.button_outside, InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(inputs.button_inside, InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(inputs.sensor_closed, InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(inputs.sensor_open, InputSwitch::State::INPUT_HIGH);

    
    button_outside.set_state(InputSwitch::State::INPUT_HIGH);
    button_inside.set_state(InputSwitch::State::INPUT_LOW);
    lightbarrier_closed.set_state(InputSwitch::State::INPUT_HIGH);
    lightbarrier_open.set_state(InputSwitch::State::INPUT_LOW);

    inputs = inputs_set.get_inputs();

    // check state and output
    ASSERT_EQ(inputs.button_outside, InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(inputs.button_inside, InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(inputs.sensor_closed, InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(inputs.sensor_open, InputSwitch::State::INPUT_LOW);
    
}

TEST(input_suite, event_init)
{
    
    InputSwitchMock button_outside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock button_inside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock lightbarrier_closed(InputSwitch::State::INPUT_LOW);  // <-- door in "closed" position
    InputSwitchMock lightbarrier_open(InputSwitch::State::INPUT_HIGH);   // <-- door not in "opened" position

    AnalogSensorMock pressureSensor;
    AnalogSensorEventGenerator pressureSensorEG(&pressureSensor);

    // create Input struct
    TimeSpec time;
    Inputs inputs_set(&button_outside, &button_inside, &lightbarrier_closed, &lightbarrier_open, &pressureSensorEG, time);

    input_t inputs;
    events_t events;

    inputs = inputs_set.get_inputs();
    events = inputs_set.get_events();


    // check state and output
    ASSERT_EQ(events.button_inside_pressed, EdgeDetector::NONE);
    ASSERT_EQ(events.button_outside_pressed, EdgeDetector::NONE);
    ASSERT_EQ(events.light_barrier_closed, EdgeDetector::NONE);
    ASSERT_EQ(events.light_barrier_open, EdgeDetector::NONE);
}

TEST(input_suite, event_switch)
{
    
    InputSwitchMock button_outside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock button_inside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock lightbarrier_closed(InputSwitch::State::INPUT_LOW);  // <-- door in "closed" position
    InputSwitchMock lightbarrier_open(InputSwitch::State::INPUT_HIGH);   // <-- door not in "opened" position

    AnalogSensorMock pressureSensor;
    AnalogSensorEventGenerator pressureSensorEG(&pressureSensor);

    // create Input struct
    TimeSpec time;
    Inputs inputs_set(&button_outside, &button_inside, &lightbarrier_closed, &lightbarrier_open, &pressureSensorEG, time);

    input_t inputs;
    events_t events;

    inputs = inputs_set.get_inputs();
    events = inputs_set.get_events();


    // check state and output
    ASSERT_EQ(events.button_inside_pressed, EdgeDetector::NONE);
    ASSERT_EQ(events.button_outside_pressed, EdgeDetector::NONE);

    button_outside.set_state(InputSwitch::State::INPUT_HIGH);
    button_inside.set_state(InputSwitch::State::INPUT_HIGH);
    lightbarrier_closed.set_state(InputSwitch::State::INPUT_HIGH);
    lightbarrier_open.set_state(InputSwitch::State::INPUT_LOW);

    events = inputs_set.get_events();

    ASSERT_EQ(events.button_inside_pressed, EdgeDetector::RISING);
    ASSERT_EQ(events.button_outside_pressed, EdgeDetector::RISING);
    ASSERT_EQ(events.light_barrier_closed, EdgeDetector::RISING);
    ASSERT_EQ(events.light_barrier_open, EdgeDetector::FALLING);

    events = inputs_set.get_events();

    ASSERT_EQ(events.button_inside_pressed, EdgeDetector::NONE);
    ASSERT_EQ(events.button_outside_pressed, EdgeDetector::NONE);
    ASSERT_EQ(events.light_barrier_closed, EdgeDetector::NONE);
    ASSERT_EQ(events.light_barrier_open, EdgeDetector::NONE);

}


// ---Test for Pressure-Sensor---

TEST(input_suite, Pressure_sensor_init)
{
    
    InputSwitchMock button_outside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock button_inside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock lightbarrier_closed(InputSwitch::State::INPUT_LOW);    // door in "closed" position
    InputSwitchMock lightbarrier_open(InputSwitch::State::INPUT_HIGH);   // door not in "opened" position
    AnalogSensorMock pressureSensor;
    AnalogSensorEventGenerator pressureSensorEG(&pressureSensor);

    // create Input struct
    TimeSpec time;
    Inputs inputs_set(&button_outside, &button_inside, &lightbarrier_closed, &lightbarrier_open, &pressureSensorEG, time);

    input_t inputs;
    events_t events;

    inputs = inputs_set.get_inputs();
    events = inputs_set.get_events();

    // check state and output
    ASSERT_EQ(inputs.analogsensor, AnalogSensorEvent::NORMAL_VALUE);
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::NORMAL_VALUE);

}

TEST(input_suite, Pressure_sensor_test)
{

    AnalogSensorMock pressureSensor;

    pressureSensor.set_value(25.12);

    ASSERT_NEAR(pressureSensor.get_value(),25.12,0.001);

};

TEST(input_suite, Pressure_sensor_event_generator_test)
{
    InputSwitchMock button_outside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock button_inside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock lightbarrier_closed(InputSwitch::State::INPUT_LOW);    // door in "closed" position
    InputSwitchMock lightbarrier_open(InputSwitch::State::INPUT_HIGH);   // door not in "opened" position
    AnalogSensorMock pressureSensor;

    // set values for over- and under-pressure
    AnalogSensorEventGenerator pressureSensorEG(&pressureSensor, 20.0f, 30.0f);

    // create Input struct
    TimeSpec time;
    Inputs inputs_set(&button_outside, &button_inside, &lightbarrier_closed, &lightbarrier_open, &pressureSensorEG, time);

    events_t events;

    events = inputs_set.get_events();

    //testing with different pressure values
    pressureSensor.set_value(25.12);
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::NORMAL_VALUE);

    pressureSensor.set_value(15.56f);
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::UNDER_VALUE);

    pressureSensor.set_value(35.0f);
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::OVER_VALUE);

    pressureSensor.set_value(-5.0f);
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::ANALOG_SENSOR_ERROR);
}

TEST(input_suite, Pressure_sensor_event_generator_hysteresis_test)
{
    InputSwitchMock button_outside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock button_inside(InputSwitch::State::INPUT_LOW);
    InputSwitchMock lightbarrier_closed(InputSwitch::State::INPUT_LOW);    // door in "closed" position
    InputSwitchMock lightbarrier_open(InputSwitch::State::INPUT_HIGH);   // door not in "opened" position
    AnalogSensorMock pressureSensor;

    // thresholds
    float under = 1.0f;
    float over = 3.0f;

    // enable hysteresis with margin 0.2
    AnalogSensorEventGenerator pressureSensorEG(&pressureSensor, under, over, true, 0.2f);

    // create Input struct
    TimeSpec time;
    Inputs inputs_set(&button_outside, &button_inside, &lightbarrier_closed, &lightbarrier_open, &pressureSensorEG, time);

    events_t events;
  
    // start in normal
    pressureSensor.set_value(2.0f);
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::NORMAL_VALUE);

    // small rise below hysteresis margin -> stay NORMAL
    pressureSensor.set_value(3.05f); // over + h = 3.2 -> should remain NORMAL
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::NORMAL_VALUE);

    // exceed hysteresis margin -> become OVER
    pressureSensor.set_value(3.25f);
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::OVER_VALUE);

    // small drop but still above over - h -> remain OVER
    pressureSensor.set_value(3.1f); // over - h = 2.8 -> still OVER
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::OVER_VALUE);

    // drop below over - h -> back to NORMAL
    pressureSensor.set_value(2.75f);
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::NORMAL_VALUE);

    // test under hysteresis
    pressureSensor.set_value(0.95f); // under - h = 0.8 -> 0.95 > 0.8 so should remain NORMAL
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::NORMAL_VALUE);

    // drop below under - h -> become UNDER
    pressureSensor.set_value(0.75f);
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::UNDER_VALUE);

    // recover above under + h -> become NORMAL
    pressureSensor.set_value(1.25f); // under + h = 1.2
    events = inputs_set.get_events();
    ASSERT_EQ(events.analog_state, AnalogSensorEvent::NORMAL_VALUE);
}
