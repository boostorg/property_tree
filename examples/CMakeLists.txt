set(EXAMPLE_NAMES
        custom_data_type
        debug_settings
#        empty_ptree_trick FIXME: compile error
        info_grammar_spirit
        speed_test
)

find_package(Threads REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.67.0 REQUIRED)

foreach(EXAMPLE_NAME ${EXAMPLE_NAMES})
    message(STATUS ${PROJECT_BINARY_DIR})

    add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}.cpp)
    target_compile_features(${EXAMPLE_NAME} PRIVATE cxx_std_11)
    target_compile_options(${EXAMPLE_NAME} PRIVATE -Wall -Wextra)
    target_include_directories(${EXAMPLE_NAME}
            PRIVATE ${Boost_INCLUDE_DIRS}
            PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    add_custom_target(example-${EXAMPLE_NAME})
    add_custom_command(TARGET example-${EXAMPLE_NAME}
            POST_BUILD
            COMMAND ${EXAMPLE_NAME}
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/examples
    )
endforeach ()
