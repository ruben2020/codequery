#
# CodeQuery
# Copyright (C) 2013-2018 ruben2020 https://github.com/ruben2020/
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

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


