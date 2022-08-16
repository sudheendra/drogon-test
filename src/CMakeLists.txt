project(Server)

find_package(CURL REQUIRED)
find_package(Drogon REQUIRED)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB SRC_LIST "*.hpp" "*.cpp")

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(SYSTEM ${DROGON_INCLUDE_DIRS})
include_directories(SYSTEM ${CURL_INCLUDE_DIRS})

link_directories(${CURL_LIBRARIES})

add_executable(drogon-client ${SRC_LIST})
target_compile_options(drogon-client PRIVATE -std=c++20 -pthread -O3)

target_link_libraries(
    drogon-client
    ${DROGON_LIBRARIES}
    ${CURL_LIBRARIES}
)
