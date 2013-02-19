cmake_minimum_required(VERSION 2.8)
project(CodeQuery)

find_package(Sqlite REQUIRED)

include_directories( "./cli" )
include_directories( "./makedb" )
include_directories( ${SQLITE_INCLUDE_DIR} )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )
set( SMALLLIB_SRCS makedb/small_lib.cpp )
add_library( small_lib STATIC ${SMALLLIB_SRCS} )

set( SQLQUERY_SRCS cli/sqlquery.cpp )
add_library( sqlquery_lib STATIC ${SQLQUERY_SRCS} )
target_link_libraries( sqlquery_lib ${SQLITE_LIBRARIES} ${SQLITE_LIBRARIES} small_lib )

add_subdirectory( makedb )
add_subdirectory( gui )


