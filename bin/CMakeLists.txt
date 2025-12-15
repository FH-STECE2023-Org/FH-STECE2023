add_executable(run-door run-door.cpp)

target_include_directories(
    run-door
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/src 
    ${CMAKE_SOURCE_DIR}/src/door/input_output_switch/input 
    ${CMAKE_SOURCE_DIR}/src/door/input_output_switch/output
)


target_link_libraries(
    run-door
    PUBLIC door                  
    PUBLIC utilities             
    PUBLIC input_output_switch   
    PUBLIC motor                
    PUBLIC analog_stuff          
)