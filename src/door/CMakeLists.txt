set(DOOR_FILES

  door.h
  door.cpp

  small_file_handling.h
  small_file_handling.cpp
  
)


add_subdirectory(utilities)
add_subdirectory(motor)
add_subdirectory(input_output_switch)
add_subdirectory(analog_stuff)

add_library(
  door

  ${DOOR_FILES}
  event-edge-detector.cpp
  event-edge-detector.h 
)


target_link_libraries(door PUBLIC motor)
target_link_libraries(door PUBLIC utilities)
target_link_libraries(door PUBLIC input_output_switch)
target_link_libraries(door PUBLIC analog_stuff)

target_include_directories(
  door
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)
