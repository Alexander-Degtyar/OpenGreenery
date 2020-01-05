cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/rpi-toolchain.cmake )
set( CMAKE_CXX_STANDARD 17 )
project( ads1115 )

set( SOURCES    src/ADS1115.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )
