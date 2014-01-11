##
## Copyright (c) 2014 Oliver Giles
## All rights reserved.
##
## This file is part of Hanchly (汉查利).
## Hanchly is free software; resdistribution and use are
## permitted under conditions stipulated in the file
## COPYING, which is distributed with Hanchly.
##
macro(add_as_obj FILENAME)
	if(IS_ABSOLUTE ${FILENAME})
		file(RELATIVE_PATH FILENAME_RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${FILENAME})
	else()
		set(FILENAME_RELATIVE ${FILENAME})
	endif()
	set(OBJ_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME_RELATIVE})
	get_filename_component(SOURCE_DIR ${OBJ_INPUT} DIRECTORY)
	get_filename_component(SOURCE_FILE ${OBJ_INPUT} NAME)
	set(OBJ_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}.o)
	get_filename_component(OUTPUT_DIR ${OBJ_OUTPUT} DIRECTORY)
	file(MAKE_DIRECTORY ${OUTPUT_DIR})
	add_custom_command(OUTPUT ${OBJ_OUTPUT}
			COMMAND objcopy -I binary -O elf32-i386 -B i386 # todo 64 bit
			${SOURCE_FILE}
			${OBJ_OUTPUT}
			DEPENDS ${OBJ_INPUT}
			WORKING_DIRECTORY ${SOURCE_DIR}
			COMMENT Generating ${OBJ_OUTPUT}
	)
	list(APPEND OBJCOPY_BINARIES ${OBJ_OUTPUT})
endmacro(add_as_obj)

