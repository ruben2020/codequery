#
# CodeQuery
# Copyright (C) 2013-2018 ruben2020 https://github.com/ruben2020/
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.16.0)
project(CodeQuery)

option(BUILD_QT6 "Qt6 option"    OFF)
option(BUILD_QT5 "Qt5 option"    ON)
option(NO_GUI    "No GUI option" OFF)
option(GHAWIN    "GitHub Actions on Windows" OFF)

MESSAGE("-- NO_GUI = " ${NO_GUI})
if (NO_GUI)
else (NO_GUI)

if (BUILD_QT6)
message("-- BUILD_QT6 = " ${BUILD_QT6})
else (BUILD_QT6)

message("-- BUILD_QT6 = " ${BUILD_QT6})
message("-- BUILD_QT5 = " ${BUILD_QT5})

if (BUILD_QT5)
else (BUILD_QT5)
message(FATAL_ERROR "Qt4 is no longer supported!")
endif(BUILD_QT5)

endif(BUILD_QT6)
endif(NO_GUI)

if (GHAWIN)
MESSAGE("-- GHAWIN = " ${GHAWIN})
else (GHAWIN)
endif(GHAWIN)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmakefind/")


# C++17 now always required
SET( CMAKE_CXX_STANDARD 17 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

#set(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_C_FLAGS_DEBUG "-g -ggdb -O0 -Wno-unused-result -Wno-return-type")
#SET(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0 -Wno-unused-result -Wno-return-type")

IF(MSVC)
ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
ELSE(MSVC)
SET( CMAKE_CXX_FLAGS  "-O2 -Wno-unused-result -Wno-return-type ${CMAKE_CXX_FLAGS}")
SET( CMAKE_CXX_FLAGS_DEBUG  "-g -ggdb -O0 -Wno-unused-result -Wno-return-type ${CMAKE_CXX_FLAGS_DEBUG}" )
SET( CMAKE_CXX_FLAGS_RELEASE  "-O2 -Wno-unused-result -Wno-return-type ${CMAKE_CXX_FLAGS_RELEASE}" )
ENDIF(MSVC)


set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )


add_subdirectory( "querylib" )
add_subdirectory( "makedb" )
add_subdirectory( "cli" )

if (NO_GUI)
else (NO_GUI)
add_subdirectory( "lexilla" )
add_subdirectory( "scintilla/qt/ScintillaEdit" )
add_subdirectory( "showgraph" )
add_subdirectory( "gui" )
endif (NO_GUI)

# Increasing minimum version numbers of dependencies to support Ubuntu 16.04 LTS (xenial)

if (UNIX)

if (NO_GUI)

set( CPACK_PACKAGE_NAME "codequery-nogui" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libsqlite3-0 (>= 3.11.0) " )

else (NO_GUI)
if (BUILD_QT6)

set( CPACK_PACKAGE_NAME "codequery-qt6" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "qtbase5-dev (>= 5.5.1) , libsqlite3-0 (>= 3.11.0) " )

else (BUILD_QT6)

set( CPACK_PACKAGE_NAME "codequery-qt5" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "qtbase5-dev (>= 5.9.0) , libsqlite3-0 (>= 3.11.0) " )

endif (BUILD_QT6)
endif (NO_GUI)


  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
  else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386" )
  endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

set( CPACK_GENERATOR "DEB;TGZ")
set( CPACK_PACKAGE_VERSION  "0.27.0" )
set( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Linux-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}" ) 
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Code understanding and searching tool")
set( CPACK_PACKAGE_DESCRIPTION "Code understanding and searching tool")
set( CPACK_PACKAGE_VENDOR "ruben2020")
set( CPACK_DEBIAN_PACKAGE_RECOMMENDS "cscope , exuberant-ctags" )
set( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
set( CPACK_DEBIAN_PACKAGE_MAINTAINER "ruben2020 ruben2020-git@users.sourceforge.net")
set( CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://ruben2020.github.io/codequery/")

include(CPack)

endif(UNIX)

