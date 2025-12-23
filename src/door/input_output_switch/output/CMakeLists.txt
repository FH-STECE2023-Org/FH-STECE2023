add_library(
    output

    output-switch-gpio-sysfs.cpp
    output-switch-gpio-sysfs.h
    output-switch-mock.cpp
    output-switch-mock.h
    output-switch.h
)

target_include_directories(
    output
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(
    output
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)