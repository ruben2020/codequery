cmake_minimum_required(VERSION 2.8)
project(CodeQuery)

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

INCLUDE_DIRECTORIES( "../makedb" )
set( OPTLISTLIB_SRCS makedb/optlist.cpp )
add_library( optlist_lib STATIC ${OPTLISTLIB_SRCS} )

add_subdirectory( "querylib" )
add_subdirectory( "makedb" )
add_subdirectory( "gui" )


