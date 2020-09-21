project( og-irrigation )

set( SRCS    src/state_machine/Context.cpp
             src/state_machine/DryState.cpp
             src/state_machine/State.cpp
             src/state_machine/WetState.cpp
             src/IrrigationController.cpp )

add_library( ${PROJECT_NAME}_impl    ${SRCS} )
target_include_directories( ${PROJECT_NAME}_impl    PUBLIC inc )
target_link_libraries( ${PROJECT_NAME}_impl    database
                                               pump
                                               tools )

add_executable( ${PROJECT_NAME}    main.cpp )
target_link_libraries( ${PROJECT_NAME}    ${PROJECT_NAME}_impl )
