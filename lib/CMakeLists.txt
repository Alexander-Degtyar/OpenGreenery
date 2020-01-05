set( LIBS ${CMAKE_CURRENT_SOURCE_DIR} )# Libs root path

add_subdirectory( driver/ads1115 )
add_subdirectory( gpio )
add_subdirectory( relay )
add_subdirectory( third-party/wiringPi )
