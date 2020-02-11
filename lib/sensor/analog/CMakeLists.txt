cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/rpi-toolchain.cmake )
set( CMAKE_CXX_STANDARD 17 )
project( analog_sensor )

set( SOURCES    src/AnalogSensor.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc
                                               ${LIBS}/adc )
find_package (Threads)
target_link_libraries( ${PROJECT_NAME}    adc
                                          Threads::Threads )
