#
# This license header applies to only this file.
# 
# Copyright (c) 2011 ruben2020 https://github.com/ruben2020/
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
# 

cmake_minimum_required(VERSION 3.16.0)
project(CodeQueryLib)

find_package(Sqlite REQUIRED)

include_directories( "." )
include_directories( "${SQLITE_INCLUDE_DIR}" )

set( SMALL_LIB_SRCS small_lib.cpp getopt2.cpp )
add_library( small_lib STATIC ${SMALL_LIB_SRCS} )

set( QUERY_LIB_SRCS sqlquery.cpp )
add_library( sqlquery_lib STATIC ${QUERY_LIB_SRCS} )
target_link_libraries( sqlquery_lib ${SQLITE_LIBRARIES} small_lib )


