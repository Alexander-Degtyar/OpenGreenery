project( tests )

find_package(GTest REQUIRED)
include_directories( ${GTEST_INCLUDE_DIRS} )
enable_testing()

set( SRCS    main.cpp
             lib/database/DatabaseTest.cpp
             lib/relay/RelayTest.cpp )

add_executable( ${PROJECT_NAME}    ${SRCS} )
target_link_libraries( ${PROJECT_NAME}    ${GTEST_LIBRARIES}
                                          database
                                          relay )
