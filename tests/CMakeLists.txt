add_executable(
  door-tests

  door-init-tests.cpp
  door-closed-open-tests.cpp
  door-open-closed-tests.cpp

  timespec-tests.cpp
  eventloop-tests.cpp
  timer_test.cpp
  
  event-edge-detector-tests.cpp

  inputs-tests.cpp
  outputs-tests.cpp

  adc-mock-tests.cpp
)
target_include_directories(
  door-tests
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  door-tests

  PRIVATE
  statemachine
  utilities
  gtest_main

  input_output_switch
  motor
  analog_stuff
)
add_test(
  NAME door-tests 
  COMMAND door-tests
)

add_executable(
  motor-tests   
  motor-tests.cpp
)
target_include_directories(
  motor-tests
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  motor-tests
  statemachine                
)

# add_executable(motorLED-tests motorLED-tests.cpp)
# target_link_libraries(
#   motorLED-tests
#   statemachine
#   gtest_main
# )

add_executable(
  input-switch-gpio-tests input-switch-gpio-tests.cpp
)
target_include_directories(
  input-switch-gpio-tests
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  input-switch-gpio-tests

  statemachine
  gtest_main
)

# add_executable(motor-stepper-tests motor-stepper-tests.cpp)
# target_link_libraries(
#   motor-stepper-tests

#   statemachine
#   gtest_main
# )


add_executable(
  input-output-switch-gpio-sysfs-tests
  input-output-switch-gpio-sysfs-tests.cpp
)
target_include_directories(
  input-output-switch-gpio-sysfs-tests
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  input-output-switch-gpio-sysfs-tests
  statemachine
  gtest_main
)
add_test(
  NAME input-output-switch-gpio-sysfs-tests
  COMMAND sudo input-output-switch-gpio-sysfs-tests
)

add_executable(bmp280_test bmp280-test.cpp)
add_executable(adc_ads1115_test adc-ads1115-test.cpp)
add_executable(hysteresis_hw_test hysteresis_hw_test.cpp)
add_executable(input-edge-event-test input-edge-event-test.cpp)

target_include_directories(
  bmp280_test
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  bmp280_test

  PRIVATE
  statemachine
  gtest_main
)

target_include_directories(
  adc_ads1115_test
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  adc_ads1115_test

  PRIVATE
  statemachine
  gtest_main
)

target_include_directories(
  hysteresis_hw_test
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  hysteresis_hw_test

  PRIVATE
  statemachine
)

target_include_directories(
  input-edge-event-test
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  input-edge-event-test

  PRIVATE
  statemachine
  utilities
)

add_test(
  NAME bmp280_test
  COMMAND bmp280_test
)

add_test(
  NAME adc_ads1115_test
  COMMAND adc_ads1115_test
)