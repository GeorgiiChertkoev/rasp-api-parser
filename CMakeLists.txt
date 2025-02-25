cmake_minimum_required(VERSION 3.12)

project(
    labwork6
    VERSION 1.0
    DESCRIPTION "C++ Year 2025 labwork6"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.11.1)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
# FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git
                        #   GIT_TAG v3.11.3)
FetchContent_MakeAvailable(json)

file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/cache)



add_subdirectory(lib)
add_subdirectory(bin)