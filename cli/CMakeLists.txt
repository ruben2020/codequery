#
# CodeQuery
# Copyright (C) 2013-2018 ruben2020 https://github.com/ruben2020/
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.5.0)
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


