cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/rpi-toolchain.cmake )
set( CMAKE_CXX_STANDARD 17 )
project( gpio )

set( SOURCES    src/GPIOctl.cpp
                src/GPIOFactory.cpp
                src/OutputGPIOctl.cpp
                src/InputGPIOctl.cpp
                src/PinId.cpp
                src/Pinout.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc
                                               PUBLIC ${LIBS}/third-party/wiringPi )# Include wPi headers
target_link_libraries( ${PROJECT_NAME}    wiringPi )
