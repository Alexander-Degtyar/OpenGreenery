project( relay )

set( SOURCES    src/Relay.cpp )

add_library( ${PROJECT_NAME}    ${SOURCES} )
target_include_directories( ${PROJECT_NAME}    PUBLIC inc
                                               ${LIBS}/gpio )# Include GPIO headers
target_link_libraries( ${PROJECT_NAME}    gpio )
