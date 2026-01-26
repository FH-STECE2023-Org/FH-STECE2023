add_library(
    statemachine

    door.h
    door.cpp
    structs.h
    inputs.h
    inputs.cpp
    outputs.h
    outputs.cpp
)

target_link_libraries(
    statemachine

    PUBLIC

    motor
    utilities
    input
    output
    sensor
)

target_include_directories(
    statemachine

    PUBLIC

    ${CMAKE_SOURCE_DIR}/src
)
