cmake_minimum_required(VERSION 2.6)
project(hanchly)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug or Release")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")

include(scripts/objcopy.cmake)

add_as_obj(data/pinyin_syllables)
add_as_obj(data/english_pinyin)

add_library(hanchly enpy.c pinyin.c ${OBJCOPY_BINARIES})
set_target_properties(hanchly PROPERTIES COMPILE_FLAGS "-std=gnu11")

add_subdirectory(test)
