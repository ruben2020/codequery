cmake_minimum_required(VERSION 2.8)
project(CodeQuery)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/helper/")

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

INCLUDE_DIRECTORIES( "../makedb" )
set( OPTLISTLIB_SRCS makedb/optlist.cpp )
add_library( optlist_lib STATIC ${OPTLISTLIB_SRCS} )

add_subdirectory( "querylib" )
add_subdirectory( "makedb" )
add_subdirectory( "showgraph" )
add_subdirectory( "gui" )


