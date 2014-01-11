##
## Copyright (c) 2014 Oliver Giles
## All rights reserved.
##
## This file is part of Hanchly (汉查利).
## Hanchly is free software; resdistribution and use are
## permitted under conditions stipulated in the file
## COPYING, which is distributed with Hanchly.
##
find_package(GTest)
if(NOT GTEST_FOUND)
	return()
endif()

# Set up dependencies
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/.. ${GTEST_INCLUDE_DIRS})

# Convenience macro
macro(add_gtest TESTNAME)
    add_executable(${TESTNAME} ${TESTNAME}.cpp)
    target_link_libraries(${TESTNAME} hanchly ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
    add_custom_command(TARGET ${TESTNAME} POST_BUILD COMMAND ${TESTNAME})
endmacro()

# Add tests here
add_gtest(test_pinyin)
add_gtest(test_enpy)
add_gtest(test_cedict)
