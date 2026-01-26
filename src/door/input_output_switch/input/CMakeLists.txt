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

    PUBLIC

    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    input

    PUBLIC

    utilities
)
