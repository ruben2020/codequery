cmake_minimum_required(VERSION 2.8)
project(CodeQuery)

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )
set( CODEQUERY_SRCS makedb/small_lib.cpp )
add_library( small_lib STATIC ${CODEQUERY_SRCS} )

add_subdirectory( makedb )
add_subdirectory( gui )
