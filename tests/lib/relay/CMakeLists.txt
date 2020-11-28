project(relay_unit_tests)

find_package(GTest REQUIRED)
include_directories( ${GTEST_INCLUDE_DIRS} )
add_executable( ${PROJECT_NAME}    RelayUT.cpp )
target_link_libraries( ${PROJECT_NAME}    ${GTEST_LIBRARIES}
                                          relay )
