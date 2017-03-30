cmake_minimum_required(VERSION 3.2.2)
project(CodeQueryLib)

find_package(Sqlite REQUIRED)

include_directories( "." )
include_directories( "${SQLITE_INCLUDE_DIR}" )

set( SMALL_LIB_SRCS small_lib.cpp getopt2.cpp )
add_library( small_lib STATIC ${SMALL_LIB_SRCS} )

set( QUERY_LIB_SRCS sqlquery.cpp )
add_library( sqlquery_lib STATIC ${QUERY_LIB_SRCS} )
target_link_libraries( sqlquery_lib ${SQLITE_LIBRARIES} small_lib )


