##
## Copyright (c) 2014 Oliver Giles
## All rights reserved.
##
## This file is part of Hanchly (汉查利).
## Hanchly is free software; resdistribution and use are
## permitted under conditions stipulated in the file
## COPYING, which is distributed with Hanchly.
##
cmake_minimum_required(VERSION 2.6)
project(hanchly)
include(scripts/objcopy.cmake)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")

# fetch 3rd-party resources
add_custom_command(OUTPUT
		jun_da_freq_modern.csv
		jun_da_freq_classical.csv
		cedict_1_0_ts_utf-8_mdbg.txt.gz
		Unihan.zip
		COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/scripts/download.cmake
		COMMENT "Downloading 3rd-party resources")

add_custom_command(OUTPUT char_freq COMMAND cat
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_modern.csv
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_classical.csv |
		${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_freq_table.sh >
		${CMAKE_CURRENT_BINARY_DIR}/char_freq
		DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_modern.csv
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_classical.csv
		scripts/create_freq_table.sh)

add_custom_command(OUTPUT cedict_1_0_ts_utf-8_mdbg.txt COMMAND gunzip -k
		${CMAKE_CURRENT_BINARY_DIR}/cedict_1_0_ts_utf-8_mdbg.txt.gz
		DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/cedict_1_0_ts_utf-8_mdbg.txt.gz)
add_custom_command(OUTPUT cedict COMMAND perl
		${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_dict.pl
		${CMAKE_CURRENT_BINARY_DIR}/char_freq
		${CMAKE_CURRENT_BINARY_DIR}/cedict_1_0_ts_utf-8_mdbg.txt
		${CMAKE_CURRENT_BINARY_DIR}/cedict
		DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/char_freq
		${CMAKE_CURRENT_BINARY_DIR}/cedict_1_0_ts_utf-8_mdbg.txt
		${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_dict.pl)

add_custom_command(OUTPUT
		Unihan_Readings.txt
		Unihan_DictionaryLikeData.txt
		Unihan_RadicalStrokeCounts.txt
		COMMAND unzip -o ${CMAKE_CURRENT_BINARY_DIR}/Unihan.zip
		# the files are older than the archive, this confuses make
		COMMAND touch Unihan_Readings.txt Unihan_DictionaryLikeData.txt Unihan_RadicalStrokeCounts.txt
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Unihan.zip)
		
add_custom_command(OUTPUT unihan_data unihan_idx COMMAND perl
		${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_unihan.pl
		${CMAKE_CURRENT_BINARY_DIR}/char_freq
		${CMAKE_CURRENT_BINARY_DIR}/unihan_idx
		${CMAKE_CURRENT_BINARY_DIR}/unihan_data
		${CMAKE_CURRENT_BINARY_DIR}/Unihan_Readings.txt
		${CMAKE_CURRENT_BINARY_DIR}/Unihan_DictionaryLikeData.txt
		${CMAKE_CURRENT_BINARY_DIR}/Unihan_RadicalStrokeCounts.txt
		DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/char_freq
		${CMAKE_CURRENT_BINARY_DIR}/Unihan_Readings.txt
		${CMAKE_CURRENT_BINARY_DIR}/Unihan_DictionaryLikeData.txt
		${CMAKE_CURRENT_BINARY_DIR}/Unihan_RadicalStrokeCounts.txt
		${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_unihan.pl)

add_as_obj(data/pinyin_syllables)
add_as_obj(data/english_pinyin)
add_as_obj(${CMAKE_CURRENT_BINARY_DIR}/char_freq)
add_as_obj(${CMAKE_CURRENT_BINARY_DIR}/cedict)
add_as_obj(${CMAKE_CURRENT_BINARY_DIR}/unihan_data)
add_as_obj(${CMAKE_CURRENT_BINARY_DIR}/unihan_idx)

add_library(hanchly dict.c enpy.c pinyin.c unihan.c ${OBJCOPY_BINARIES})
set_target_properties(hanchly PROPERTIES COMPILE_FLAGS "-std=gnu11")
set_target_properties(hanchly PROPERTIES COMPILE_DEFINITIONS "_GNU_SOURCE")

add_subdirectory(test)

add_executable(hanchly-cli cli.c)
set_target_properties(hanchly-cli PROPERTIES COMPILE_FLAGS "-std=gnu11")
target_link_libraries(hanchly-cli hanchly)
