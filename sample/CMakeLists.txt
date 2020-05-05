project( sample )

# Build ADC sample
add_executable( adc_sample    adc_sample.cpp )
target_link_libraries( adc_sample    adc )

# Build GPIO sample
add_executable( gpio_sample    gpio_sample.cpp )
target_link_libraries( gpio_sample    gpio )

# Build pump sample
add_executable( pump_sample    pump_sample.cpp )
target_link_libraries( pump_sample    pump )

# Build relay sample
add_executable( relay_sample    relay_sample.cpp )
target_link_libraries( relay_sample    relay )

# Build sensor sample
add_executable( sensor_sample    sensor_sample.cpp )
target_link_libraries( sensor_sample    analog_sensor )
