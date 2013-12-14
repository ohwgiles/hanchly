

macro(add_as_obj FILENAME)
	file(GLOB FILENAME_RELATIVE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${FILENAME})
	set(OBJ_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME_RELATIVE})
	get_filename_component(SOURCE_DIR ${OBJ_INPUT} DIRECTORY)
	get_filename_component(SOURCE_FILE ${OBJ_INPUT} NAME)
	set(OBJ_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME_RELATIVE}.o)
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

