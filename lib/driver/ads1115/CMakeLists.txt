project( ads1115 )

set( SOURCES    src/ADS1115.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc )
