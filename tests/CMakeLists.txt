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
target_link_libraries(
  door-tests

  PRIVATE

  gtest_main
  statemachine
  utilities
  input
  output
  motor
)
add_test(
  NAME door-tests 
  COMMAND door-tests
)

add_executable(
  motor-tests   
  motor-tests.cpp
)
target_link_libraries(
  motor-tests
  statemachine                
)

add_executable(
  bmp280-unit-tests
  bmp280-unit-test.cpp
)

target_link_libraries(
  bmp280-unit-tests

  PRIVATE

  utilities
  sensor
  gtest_main
)

add_executable(
  input-switch-gpio-tests input-switch-gpio-tests.cpp
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
add_executable(hysteresis_hw_test_subprocess hysteresis_hw_test_subprocess.cpp)
add_executable(input-edge-event-test input-edge-event-test.cpp)

target_link_libraries(
  bmp280_test

  PRIVATE

  statemachine
  gtest_main
)

target_link_libraries(
  adc_ads1115_test

  PRIVATE

  statemachine
  gtest_main
)

target_link_libraries(
  hysteresis_hw_test

  PRIVATE

  statemachine
)

target_link_libraries(
  hysteresis_hw_test_subprocess

  PRIVATE

  statemachine
)

target_link_libraries(
  input-edge-event-test

  PRIVATE

  statemachine
  utilities
)
