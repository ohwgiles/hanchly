cmake_minimum_required(VERSION 2.6)
project(hanchly)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug or Release")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")

add_subdirectory(test)
