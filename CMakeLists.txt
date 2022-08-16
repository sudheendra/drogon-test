cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0074 NEW)

project(Worker VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(fetch_packages)

include(FetchContent)
include(CPM)

cpmaddpackage("gh:TheLartians/Format.cmake@1.7.3")

add_definitions(
    -DNOMINMAX -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}" -DPROJECT_VERSION="${PROJECT_VERSION}"
)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

add_subdirectory(src)

