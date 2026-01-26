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

    PUBLIC

    output
)

target_include_directories(
    motor

    PUBLIC

    ${CMAKE_SOURCE_DIR}/src
)
