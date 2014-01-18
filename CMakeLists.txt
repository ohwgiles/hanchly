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

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug or Release")

#set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")

# Fetch Jun Da Frequency List tables. Tables are provided
# for research and teaching/learning purposes; commercial
# use must have prior written permission. See COPYING
file(DOWNLOAD "http://lingua.mtsu.edu/chinese-computing/statistics/char/download.php?Which=MO"
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_modern.csv
		EXPECTED_MD5 fe5f1d5b9e3d147fd82a517742e20438
		SHOW_PROGRESS)
file(DOWNLOAD "http://lingua.mtsu.edu/chinese-computing/statistics/char/download.php?Which=CL"
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_classical.csv
		EXPECTED_MD5 aad0764d17a1f4e2567c9bf9a80a4209
		SHOW_PROGRESS)
add_custom_command(OUTPUT char_freq COMMAND cat
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_modern.csv
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_classical.csv |
		${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_freq_table.sh >
		${CMAKE_CURRENT_BINARY_DIR}/char_freq
		DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_modern.csv
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_classical.csv
		scripts/create_freq_table.sh)

# Fetch CC-CEDICT. CC-CEDICT is licensed under the CC-BY-SA 3.0
# See COPYING
file(DOWNLOAD "http://www.mdbg.net/chindict/export/cedict/cedict_1_0_ts_utf-8_mdbg.txt.gz"
		${CMAKE_CURRENT_BINARY_DIR}/cedict_1_0_ts_utf-8_mdbg.txt.gz
		EXPECTED_MD5 2e346114d4714f9358144aa8b1a6f8ad
		SHOW_PROGRESS)
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
# Fetch the Unihan databese. Provided as-is by the Unicode
# Consortium. See COPYING
file(DOWNLOAD "http://www.unicode.org/Public/UCD/latest/ucd/Unihan.zip"
		${CMAKE_CURRENT_BINARY_DIR}/Unihan.zip
		EXPECTED_MD5 36a62127fc62cc5b617f2bf4bc72b7fa
		SHOW_PROGRESS)
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

include(scripts/objcopy.cmake)

add_as_obj(data/pinyin_syllables)
add_as_obj(data/english_pinyin)
add_as_obj(${CMAKE_CURRENT_BINARY_DIR}/char_freq)
add_as_obj(${CMAKE_CURRENT_BINARY_DIR}/cedict)
add_as_obj(${CMAKE_CURRENT_BINARY_DIR}/unihan_data)
add_as_obj(${CMAKE_CURRENT_BINARY_DIR}/unihan_idx)

add_library(hanchly cedict.c enpy.c pinyin.c unihan.c ${OBJCOPY_BINARIES})
set_target_properties(hanchly PROPERTIES COMPILE_FLAGS "-std=gnu11")
set_target_properties(hanchly PROPERTIES COMPILE_DEFINITIONS "_GNU_SOURCE")

add_subdirectory(test)

add_executable(hanchly-cli cli.c)
set_target_properties(hanchly-cli PROPERTIES COMPILE_FLAGS "-std=gnu11")
target_link_libraries(hanchly-cli hanchly)
