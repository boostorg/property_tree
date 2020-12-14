set(TEST_NAMES
        sandbox # FIXME: test fails
        test_info_parser
        test_ini_parser
        test_property_tree # FIXME: test fails
        test_xml_parser_rapidxml
)

find_package(Threads REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.67.0 REQUIRED COMPONENTS
        serialization
        unit_test_framework
)

function(make_test TEST_NAME)
    target_compile_features(${TEST_NAME} PRIVATE cxx_std_11)
    target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra)
    target_include_directories(${TEST_NAME}
            PRIVATE ${Boost_INCLUDE_DIRS}
            PRIVATE ${CMAKE_SOURCE_DIR}/include
            PRIVATE ${CMAKE_SOURCE_DIR}/test
    )

    if (${TEST_NAME} STREQUAL test_property_tree)
        target_link_libraries(${TEST_NAME} ${Boost_SERIALIZATION_LIBRARY})
    endif()

    if (${TEST_NAME} STREQUAL test_json_parser OR ${TEST_NAME} STREQUAL test_info_parser)
        target_link_libraries(${TEST_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    make_test(${TEST_NAME})
endforeach ()

add_executable(test_multi_module
        test_multi_module1.cpp
        test_multi_module2.cpp
)
make_test(test_multi_module)

add_executable(test_json_parser
        test_json_parser.cpp
        test_json_parser2.cpp
)
make_test(test_json_parser)

enable_testing()
