add_library(
    utilities

    timespec.h
    timespec.cpp
    eventloop.h
    eventloop.cpp
    periodic-timer.cpp
    periodic-timer.h
    one-shot-timer.cpp
    one-shot-timer.h
    graceful-term.cpp
    graceful-term.h
    i2c.h
    i2c.cpp
    i2c-interface.h
    i2c-real.h
    i2c-real.cpp
    i2c-mock.h
    i2c-mock.cpp
    event-edge-detector.h
    event-edge-detector.cpp
    small_file_handling.h
    small_file_handling.cpp
)

target_include_directories(
    utilities
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(
    utilities
    PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)
