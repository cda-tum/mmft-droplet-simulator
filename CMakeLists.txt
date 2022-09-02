cmake_minimum_required(VERSION 3.21.0)

project(droplet-simulator
    VERSION 1.0.0
    DESCRIPTION "Simulator for droplet-based microfluidic devices"
    HOMEPAGE_URL "https://iic.jku.at/eda/research/microfluidics/"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# download external libraries
include(FetchContent)
FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.3.9
)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(eigen googletest json)

find_package(Doxygen)
option(BUILD_DOC "Build documentation" OFF)

if(BUILD_DOC)
    if(DOXYGEN_FOUND)
        set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
        set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

        configure_file(${doxyfile_in} ${doxyfile} @ONLY)

        add_custom_target(documentation ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)

        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html DESTINATION share/doc)
    else(DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()
endif()

# add library
set(TARGET_NAME droplet)
add_library(${TARGET_NAME})

# add public headers
# add public headers for public access, i.e., they must be included with: #include "droplet-simulator/*.h"
target_include_directories(${TARGET_NAME} PUBLIC include)

# add public headers for internal access, i.e., they can be included directly with: #include "*.h"
target_include_directories(${TARGET_NAME} PRIVATE include/droplet-simulator)

# add sources
add_subdirectory(src)

# create executable and tests (if build as main project)
if(PROJECT_IS_TOP_LEVEL)
    # main executable
    add_executable(dropletMain)
    target_sources(dropletMain PUBLIC src/main.cpp)
    target_link_libraries(dropletMain PRIVATE ${TARGET_NAME})
    
    # tests
    enable_testing()
    include(GoogleTest)
    set(TARGET_NAME dropletTest)
    add_executable(${TARGET_NAME})
    target_include_directories(${TARGET_NAME} PUBLIC include)
    target_include_directories(${TARGET_NAME} PRIVATE include/droplet-simulator)
    target_link_libraries(${TARGET_NAME} PUBLIC gtest gtest_main)
    target_link_libraries(${TARGET_NAME} PRIVATE droplet)
    add_subdirectory(src ${CMAKE_BINARY_DIR}/test)
    add_subdirectory(tests)
    gtest_discover_tests(${TARGET_NAME})
endif()