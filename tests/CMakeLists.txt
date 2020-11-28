project( tests )

add_subdirectory(lib)

find_package(GTest REQUIRED)
include_directories( ${GTEST_INCLUDE_DIRS} )
add_executable( database_${PROJECT_NAME}    database_tests.cpp )
target_link_libraries( database_${PROJECT_NAME}    ${GTEST_LIBRARIES} database )
