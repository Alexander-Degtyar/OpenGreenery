project( analog_sensor )

set( SOURCES    src/AnalogSensorPublisher.cpp
                src/ConnectableAnalogSensorPublisher.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )
target_link_libraries( ${PROJECT_NAME}    adc
                                          dataflow
                                          gpio
                                          tools )
