set( LIBS ${CMAKE_CURRENT_SOURCE_DIR} )# Libs root path

add_subdirectory( adc )
add_subdirectory( driver/ads1115 )
add_subdirectory( gpio )
add_subdirectory( pump )
add_subdirectory( relay )
add_subdirectory( sensor/analog )
add_subdirectory( third-party/wiringPi )
