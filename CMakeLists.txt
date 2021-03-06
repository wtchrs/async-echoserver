cmake_minimum_required(VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE
    $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")

project(async-echoserver VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")

find_package(asio CONFIG REQUIRED)

include_directories(includes)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SERVER_OUTPUT async-server-${CMAKE_PROJECT_VERSION})
set(CLIENT_OUTPUT async-client-${CMAKE_PROJECT_VERSION})

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)
