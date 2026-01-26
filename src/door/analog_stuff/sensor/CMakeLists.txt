add_library(
    sensor

    analog-sensor-event-generator.cpp
    analog-sensor-event-generator.h
    analog-sensor-mock.cpp
    analog-sensor-mock.h
    analog-sensor.h
    pressure-sensor-bmp280.cpp
    pressure-sensor-bmp280.h
)

target_link_libraries(
    sensor

    PUBLIC

    adc
    utilities
)

target_include_directories(
    sensor

    PUBLIC

    ${CMAKE_SOURCE_DIR}/src
)
