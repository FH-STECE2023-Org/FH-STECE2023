#include <door/utilities/timespec.h>
#include <iostream>
#include <gtest/gtest.h>
#include <time.h>
#include <door/input_output_switch/input/input-switch-mock.h>
#include <door/utilities/event-edge-detector.h>

TEST(eventedge_suite, rising_edge)
{
    const TimeSpec debounce(0, 0);
    TimeSpec time(0, 500);
    InputSwitchMock input(InputSwitch::State::INPUT_LOW);
    EdgeDetector edge(&input, debounce); 
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::NONE);
    
    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::RISING);

}

TEST(eventedge_suite, falling_edge)
{
    const TimeSpec debounce(0, 0);
    TimeSpec time(0, 500);
    InputSwitchMock input(InputSwitch::State::INPUT_HIGH);
    EdgeDetector edge(&input, debounce);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::NONE);
    
    input.set_state(InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::FALLING);
}

TEST(eventedge_suite, none_edge)
{
    const TimeSpec debounce(0, 0);
    TimeSpec time;
    InputSwitchMock input(InputSwitch::State::INPUT_LOW);
    EdgeDetector edge(&input, time); 
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::NONE);
    
    input.set_state(InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::NONE);

    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::RISING);
    
    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::NONE);
    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::NONE);
    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::NONE);
    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(time), EdgeDetector::State::NONE);
}

TEST(eventedge_suite, debounce_rising)
{
    InputSwitchMock input(InputSwitch::State::INPUT_LOW);
    const TimeSpec debounce(0, 1000*1000*100);
    TimeSpec now;
    EdgeDetector edge(&input, debounce); 

    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::NONE);

    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::NONE);
     
    now.tv_nsec = 1000*1000*200;
    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::RISING);
    input.set_state(InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::NONE);



}

TEST(eventedge_suite, debounce_falling)
{
    InputSwitchMock input(InputSwitch::State::INPUT_HIGH);
    const TimeSpec debounce(0, 1000*1000*100);
    TimeSpec now;
    EdgeDetector edge(&input, debounce);

    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::NONE);

    input.set_state(InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::NONE);
     
    now.tv_nsec = 1000*1000*200;
    input.set_state(InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::FALLING);
    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::NONE);
}

TEST(eventedge_suite, time_smaller_than_debounce_time)
{
    InputSwitchMock input(InputSwitch::State::INPUT_HIGH);
    const TimeSpec debounce(0, 1000*1000*300);
    TimeSpec now;
    EdgeDetector edge(&input, debounce);

    now.tv_nsec = 1000*1000*200;
    input.set_state(InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::NONE);

    now.tv_nsec = 1000*1000*300;
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::FALLING);
}

TEST(eventedge_suite, debounce_jitter)
{
    InputSwitchMock input(InputSwitch::State::INPUT_LOW);
    const TimeSpec debounce(0, 1000*1000*300);
    TimeSpec now(0, 1000*1000*300);
    EdgeDetector edge(&input, debounce);

    input.set_state(InputSwitch::State::INPUT_HIGH);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::RISING);
    input.set_state(InputSwitch::State::INPUT_HIGH);
    input.set_state(InputSwitch::State::INPUT_LOW);
    input.set_state(InputSwitch::State::INPUT_HIGH);
    input.set_state(InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::NONE);

    now.tv_nsec = 1000*1000*700;
    input.set_state(InputSwitch::State::INPUT_LOW);
    ASSERT_EQ(edge.detect_edge(now), EdgeDetector::State::FALLING);
    
}