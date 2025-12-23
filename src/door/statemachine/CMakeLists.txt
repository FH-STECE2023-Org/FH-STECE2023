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
    PUBLIC motor
    PUBLIC utilities
    PUBLIC input_output_switch
    PUBLIC analog_stuff
)

target_include_directories(
    statemachine
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(
    statemachine
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)