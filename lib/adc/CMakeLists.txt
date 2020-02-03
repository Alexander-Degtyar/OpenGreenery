cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/rpi-toolchain.cmake )
set( CMAKE_CXX_STANDARD 17 )
project( adc )

set( SOURCES    src/ADCFactory.cpp
                src/ADS1115Reader.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc
                                               ${LIBS}/driver/ads1115 )
target_link_libraries( ${PROJECT_NAME}    ads1115 )
