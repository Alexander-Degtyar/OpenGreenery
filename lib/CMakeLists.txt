set( LIBS ${CMAKE_CURRENT_SOURCE_DIR} )# Libs root path

add_subdirectory( adc )
add_subdirectory( database )
add_subdirectory( dataflow )
add_subdirectory( driver/ads1115 )
add_subdirectory( gpio )
add_subdirectory( pump )
add_subdirectory( relay )
add_subdirectory( sensor/analog )
add_subdirectory( third-party/SQLiteCpp )
add_subdirectory( third-party/wiringPi )
add_subdirectory( third-party/spdlog )
add_subdirectory( tools )
