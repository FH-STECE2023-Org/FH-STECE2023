add_library(
    adc

    adc-ads1115.cpp
    adc-ads1115.h
)

target_include_directories(
    adc
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(
    adc
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(adc PUBLIC utilities)