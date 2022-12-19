#
# CodeQuery
# Copyright (C) 2013-2018 ruben2020 https://github.com/ruben2020/
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.2.2)
project(CodeQuery)


option(BUILD_QT5 "Qt5 option"    ON)
option(NO_GUI    "No GUI option" OFF)
option(GHAWIN    "GitHub Actions on Windows" OFF)

MESSAGE("-- NO_GUI = " ${NO_GUI})
if (NO_GUI)
else (NO_GUI)
MESSAGE("-- BUILD_QT5 = " ${BUILD_QT5})
endif(NO_GUI)

if (GHAWIN)
MESSAGE("-- GHAWIN = " ${GHAWIN})
else (GHAWIN)
endif(GHAWIN)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmakefind/")


# C++11 now always required
SET( CMAKE_CXX_STANDARD 11 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

IF(MSVC)
ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
ELSE(MSVC)
SET( CMAKE_CXX_FLAGS  "-O2 -Wno-unused-result" )
SET( CMAKE_CXX_FLAGS_DEBUG  "-O2 -Wno-unused-result" )
SET( CMAKE_CXX_FLAGS_RELEASE  "-O2 -Wno-unused-result" )
ENDIF(MSVC)


set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )


add_subdirectory( "querylib" )
add_subdirectory( "makedb" )
add_subdirectory( "cli" )

if (NO_GUI)
else (NO_GUI)
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
if (BUILD_QT5)

set( CPACK_PACKAGE_NAME "codequery-qt5" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "qtbase5-dev (>= 5.5.1) , libsqlite3-0 (>= 3.11.0) " )

else (BUILD_QT5)

set( CPACK_PACKAGE_NAME "codequery-qt4" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libqtcore4 (>= 4.8.7), libqtgui4 (>= 4.8.7) , libqt4-xml (>= 4.8.7) , libsqlite3-0 (>= 3.11.0) " )

endif (BUILD_QT5)
endif (NO_GUI)


  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
  else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386" )
  endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

set( CPACK_GENERATOR "DEB;TGZ")
set( CPACK_PACKAGE_VERSION  "0.26.0" )
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

