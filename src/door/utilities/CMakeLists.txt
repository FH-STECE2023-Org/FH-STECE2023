add_library(
    utilities

    timespec.h
    timespec.cpp
    eventloop.h
    eventloop.cpp
    i2c.h
    i2c.cpp
)

target_include_directories(
    utilities
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
