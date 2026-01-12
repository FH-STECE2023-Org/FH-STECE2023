add_library(
    input

    input-edge-event.cpp
    input-edge-event.h
    input-switch-gpio-sysfs.cpp
    input-switch-gpio-sysfs.h
    input-switch-mock.cpp
    input-switch-mock.h
    input-switch.h
)

target_include_directories(
    input
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(
    input
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    input
    PRIVATE utilities
    PRIVATE motor
    PRIVATE analog_stuff
)