add_executable(run-door run-door.cpp)

target_include_directories(
    run-door
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/src 
)


target_link_libraries(
    run-door
    PUBLIC statemachine
    PUBLIC utilities             
    PUBLIC input_output_switch   
    PUBLIC motor                
    PUBLIC analog_stuff          
)
