add_library(
    adc

    adc-ads1115.cpp
    adc-ads1115.h
)

target_include_directories(
    adc

    PUBLIC

    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(adc PUBLIC utilities)
