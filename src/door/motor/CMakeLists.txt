add_library(
    motor

    motor.h
    motor-mock.h
    motor-mock.cpp
    motor-stepper.h
    motor-stepper.cpp
    motorLED.h
    motorLED.cpp
    
)

target_link_libraries(
    motor 
    PRIVATE input_output_switch
)

target_include_directories(
    motor
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(
    motor
    INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)