cmake_minimum_required( VERSION 3.20 )

set( EXE_NAME cjson )
project( ${EXE_NAME} )

set( CMAKE_C_STANDARD 99 )
set( CMAKE_C_FLAGS "-Wall" )

set( SOURCE_FILES src/io.c
   				src/search.c
				src/lexer.c
				src/verify.c
				src/parse.c
				src/str.c)

add_executable( ${EXE_NAME} main.c ${SOURCE_FILES} )
target_include_directories( ${EXE_NAME} PUBLIC include/ )
target_compile_definitions( ${EXE_NAME} PUBLIC CJSON_RESOURCE_PATH="../" )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	target_compile_definitions( ${EXE_NAME} PUBLIC -DCJSON_DEBUG )
endif()

add_executable( test test/test.c ${SOURCE_FILES} )
target_include_directories( test PUBLIC include/ test/ )
