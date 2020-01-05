cmake_minimum_required( VERSION 3.0 )
set( CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/rpi-toolchain.cmake )
set ( CMAKE_C_STANDARD 11 )
project( wiringPi )

# Have CMake find our pthreads library within our toolchain (required for this library)
set( CMAKE_THREAD_PREFER_PTHREAD    TRUE )
find_package( Threads    REQUIRED )
if( NOT Threads_FOUND )
	message( SEND_ERROR    "Failed to find Threads" )
	return()
endif()

set( SOURCES WiringPi/wiringPi )
FILE( GLOB SRC_FILES    ${SOURCES}/*.c )
add_library( ${PROJECT_NAME}    SHARED ${SRC_FILES})
target_include_directories( ${PROJECT_NAME}    PUBLIC ${SOURCES} )

# link the following required libraries:
# Threads, Math, Crypt, and RealTime
target_link_libraries( ${PROJECT_NAME}    ${CMAKE_THREAD_LIBS_INIT}
                                          crypt
                                          m
                                          rt )
