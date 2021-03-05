#
# Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
# Copyright (c) 2021 Richard Hodges (hodges.r@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/boostorg/json
#

cmake_minimum_required(VERSION 3.5...3.19)

set(BOOST_PROPERTY_TREE_VERSION 100)
if(BOOST_SUPERPROJECT_VERSION)
    set(BOOST_PROPERTY_TREE_VERSION ${BOOST_SUPERPROJECT_VERSION})
endif()

project(boost_property_tree VERSION "${BOOST_PROPERTY_TREE_VERSION}" LANGUAGES CXX)

option(BOOST_PROPERTY_TREE_BUILD_TESTS "Build boost::property_tree tests" ON)
option(BOOST_PROPERTY_TREE_BUILD_EXAMPLES "Build boost::property_tree examples" ON)

file(GLOB_RECURSE BOOST_PROPERTY_TREE_HEADERS $<$<VERSION_GREATER_EQUAL:${CMAKE_VERSION},3.12>:CONFIGURE_DEPENDS>
    include/boost/*.hpp
    include/boost/*.ipp
    include/boost/*.natvis
)

set(BOOST_PROPERTY_TREE_SOURCES
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/boost PREFIX "" FILES ${BOOST_PROPERTY_TREE_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "" FILES ${BOOST_PROPERTY_TREE_SOURCES})

# TODO: For Boost superproject, do we want to support header-only mode?
#       Then, this needs to read `add_library(boost_json INTERFACE)`
#       and related settings need to be INTERFACE-ed as well.
add_library(boost_property_tree INTERFACE)
#target_sources(boost_property_tree INTERFACE ${BOOST_PROPERTY_TREE_HEADERS} ${BOOST_PROPERTY_TREE_SOURCES})
add_library(Boost::property_tree ALIAS boost_property_tree)

#target_compile_features(boost_property_tree PUBLIC cxx_constexpr)

# TODO: For Boost superproject, this may need to be INTERFACE setting.
include(GNUInstallDirs)
if(BOOST_SUPERPROJECT_VERSION)
    target_include_directories(boost_property_tree INTERFACE include)
else()
    target_include_directories(boost_property_tree
        INTERFACE
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
endif()

if(BOOST_SUPERPROJECT_VERSION)
    #
    # Building as part of Boost superproject tree, with Boost as dependency.
    #
    target_link_libraries(boost_property_tree
        PUBLIC
            Boost::boost
            Boost::assert
            Boost::config
            Boost::container
            Boost::exception
            Boost::system
            Boost::throw_exception
            Boost::utility
    )

elseif(BOOST_PROPERTY_TREE_IN_BOOST_TREE)
    #
    # Building inside Boost tree, out of Boost superproject tree, with Boost as dependency.
    # e.g. on Travis or other CI, or when producing Visual Studio Solution and Projects.
    #
    get_filename_component(BOOST_ROOT ../.. ABSOLUTE)
    set(BOOST_INCLUDEDIR ${BOOST_ROOT})
    set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib)
    find_package(Boost COMPONENTS serialization unit_test_framework REQUIRED)
    link_libraries(Boost::serialization)


    target_include_directories(boost_property_tree INTERFACE ${BOOST_ROOT})
    target_link_directories(boost_property_tree INTERFACE ${BOOST_ROOT}/stage/lib)

else()
    #
    # Building out of Boost tree, out of Boost superproject tree, with Boost as dependency.
    # e.g. for packaging or added with add_subdirectory.
    #
    find_package(Boost REQUIRED
            #COMPONENTS container system
            )
    target_link_libraries(boost_property_tree
        INTERFACE
            Boost::boost
    )
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR AND NOT BOOST_PROPERTY_TREE_IN_BOOST_TREE)
    set_target_properties(boost_property_tree PROPERTIES EXPORT_NAME property_tree)
    install(TARGETS boost_property_tree
        EXPORT boost_property_tree_targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(EXPORT boost_property_tree_targets
        FILE boost_property_tree-targets.cmake
        NAMESPACE Boost::
        DESTINATION lib/cmake/boost_property_tree
    )

    include(CMakePackageConfigHelpers)

    configure_package_config_file(cmake/config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/boost_property_tree-config.cmake
        INSTALL_DESTINATION lib/cmake/boost_property_tree
    )

    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/boost_property_tree-config-version.cmake
        VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/boost_property_tree-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/boost_property_tree-config-version.cmake
        DESTINATION lib/cmake/boost_property_tree
    )

    install(DIRECTORY include/ DESTINATION include)
endif()

if(BOOST_PROPERTY_TREE_BUILD_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()

if(BOOST_PROPERTY_TREE_BUILD_EXAMPLES)
    if (BOOST_SUPERPROJECT_VERSION)
        message(STATUS "[property_tree] superproject build - skipping examples")
    else()
        add_subdirectory(examples)
    endif()
endif()
