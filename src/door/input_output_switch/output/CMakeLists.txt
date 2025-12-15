add_library(
    output

    output-switch-gpio-sysfs.cpp
    output-switch-gpio-sysfs.h
    output-switch-mock.cpp
    output-switch-mock.h
    output-switch.h
    outputs.cpp
    outputs.h
)

target_include_directories(
    output
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..

)

target_include_directories(
    output
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    output
    PRIVATE input  
    PRIVATE motor
    PRIVATE analog_stuff
)