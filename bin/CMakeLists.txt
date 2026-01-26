add_executable(run-door run-door.cpp)

target_link_libraries(
    run-door

    PUBLIC

    statemachine
    utilities             
    motor
    sensor
)
