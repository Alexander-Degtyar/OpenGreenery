cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/rpi-toolchain.cmake )# Cross compile toolchain
set ( CMAKE_CXX_STANDARD 17 )
project( sample )

# Build ADC sample
add_executable( adc_sample    adc_sample.cpp )
target_link_libraries( adc_sample    ads1115 )

# Build GPIO sample
add_executable( gpio_sample    gpio_sample.cpp )
target_link_libraries( gpio_sample    gpio )

# Build pump sample
add_executable( pump_sample    pump_sample.cpp )
target_link_libraries( pump_sample    pump )

# Build relay sample
add_executable( relay_sample    relay_sample.cpp )
target_link_libraries( relay_sample    relay )
