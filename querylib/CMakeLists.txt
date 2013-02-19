cmake_minimum_required(VERSION 2.8)
project(CodeQuery)

find_package(Sqlite REQUIRED)

INCLUDE_DIRECTORIES( "." )

set( SMALL_LIB_SRCS small_lib.cpp )
add_library( small_lib STATIC ${SMALL_LIB_SRCS} )

set( QUERY_LIB_SRCS sqlquery.cpp )
add_library( sqlquery_lib STATIC ${QUERY_LIB_SRCS} )
target_link_libraries( sqlquery_lib ${SQLITE_LIBRARIES} small_lib )


