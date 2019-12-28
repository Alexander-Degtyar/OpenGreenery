cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    rpi-toolchain.cmake )
set( CMAKE_CXX_STANDARD 17 )
project( gpio )

set( SOURCES    src/GPIOctl.cpp
                src/OutputGPIOctl.cpp
                src/InputGPIOctl.cpp
                src/Pinout.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )

# Link wiringPi
add_subdirectory( wiringPi )# Soft link to the wPi lib
target_link_libraries( ${PROJECT_NAME}    wiringPi )
