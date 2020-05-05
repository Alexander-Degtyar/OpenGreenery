# Define our host system
set( CMAKE_SYSTEM_NAME    Linux )
set( CMAKE_SYSTEM_VERSION    1 )

# Define the cross compiler locations
if( DEFINED ENV{OPEN_GREENERY_C_COMPILER} )
    set( CMAKE_C_COMPILER    $ENV{OPEN_GREENERY_C_COMPILER} )
else()
    message( FATAL_ERROR    "Undefined OPEN_GREENERY_C_COMPILER environment variable. Cross compiler must be specified." )
endif()

if( DEFINED ENV{OPEN_GREENERY_CXX_COMPILER} )
    set( CMAKE_CXX_COMPILER    $ENV{OPEN_GREENERY_CXX_COMPILER} )
else()
    message( FATAL_ERROR    "Undefined OPEN_GREENERY_CXX_COMPILER environment variable. Cross compiler must be specified." )
endif()

# Path to cross compilation Qt configurations
set( CMAKE_PREFIX_PATH    /usr/lib/arm-linux-gnueabihf/cmake )

set( CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra" )
set( CMAKE_CXX_STANDARD_REQUIRED    ON )
