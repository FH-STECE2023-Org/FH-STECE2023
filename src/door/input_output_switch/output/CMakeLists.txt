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

    PUBLIC

    ${CMAKE_SOURCE_DIR}/src
)
