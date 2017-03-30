cmake_minimum_required(VERSION 3.2.2)
project(CodeQueryCLI)

#add_definitions( -Wall )

find_package(Sqlite REQUIRED)

include_directories( "." )
include_directories( "../makedb" )
include_directories( "../querylib" )
include_directories( "${SQLITE_INCLUDE_DIR}" )

set( CQSEARCH_SRCS
     main_cli.cpp
   )

add_executable( cqsearch ${CQSEARCH_SRCS} )
target_link_libraries( cqsearch ${SQLITE_LIBRARIES} small_lib sqlquery_lib )

install(TARGETS cqsearch RUNTIME DESTINATION bin)


