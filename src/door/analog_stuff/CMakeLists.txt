

add_subdirectory(adc)
add_subdirectory(sensor)

add_library(analog_stuff INTERFACE)
target_link_libraries(analog_stuff INTERFACE adc sensor)