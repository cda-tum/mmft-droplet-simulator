cmake_minimum_required(VERSION 3.21.0)

project(droplet-simulator
    VERSION 1.0.0
    DESCRIPTION "Simulator for droplet-based microfluidic devices"
    HOMEPAGE_URL "https://github.com/cda-tum/mmft-droplet-simulator.git"
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
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(eigen json)

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