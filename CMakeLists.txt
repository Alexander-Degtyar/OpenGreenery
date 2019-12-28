cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    rpi-toolchain.cmake )# Cross compile toolchain
set ( CMAKE_CXX_STANDARD 17 )
project( gpio_sample )

# Build executable
set( SOURCES    main.cpp )
add_executable( ${PROJECT_NAME}    ${SOURCES} )

# Link gpio
add_subdirectory( lib/gpio )
target_link_libraries( ${PROJECT_NAME}    gpio )
