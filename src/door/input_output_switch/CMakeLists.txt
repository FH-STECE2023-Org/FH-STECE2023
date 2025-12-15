add_subdirectory(output)
add_subdirectory(input)

add_library(input_output_switch INTERFACE)

target_link_libraries(input_output_switch INTERFACE input output)