add_library(
    input

    input-edge-event.cpp
    input-edge-event.h
    input-switch-gpio-sysfs.cpp
    input-switch-gpio-sysfs.h
    input-switch-mock.cpp
    input-switch-mock.h
    input-switch.h
    inputs.cpp
    inputs.h   
)

target_include_directories(
    input
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../motor
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..

)

target_include_directories(
    input
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    input
    PRIVATE door
    PRIVATE utilities
    PRIVATE motor
    PRIVATE analog_stuff
)