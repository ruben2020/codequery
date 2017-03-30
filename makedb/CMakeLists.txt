cmake_minimum_required(VERSION 3.2.2)
project(CodeQueryDBMaker)

#add_definitions( -Wall )

find_package(Sqlite REQUIRED)

include_directories( "." )
include_directories( "../querylib" )
include_directories( "${SQLITE_INCLUDE_DIR}" )

set( CQMAKEDB_SRCS
     sqlbase.cpp
     csdbheader.cpp
     csdbparser.cpp
     cs2sq.cpp
     ctagread.cpp
     main.cpp
   )

add_executable( cqmakedb ${CQMAKEDB_SRCS} )
target_link_libraries( cqmakedb ${SQLITE_LIBRARIES} small_lib )

install(TARGETS cqmakedb RUNTIME DESTINATION bin)


