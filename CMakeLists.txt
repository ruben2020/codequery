cmake_minimum_required(VERSION 3.2.2)
project(CodeQuery)


option(BUILD_QT5 OFF)

SET( CMAKE_CXX_STANDARD 11 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

IF(MSVC)
ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
ELSE(MSVC)
SET( CMAKE_CXX_FLAGS  "-O2 -Wno-unused-result" )
SET( CMAKE_CXX_FLAGS_DEBUG  "-O2 -Wno-unused-result" )
SET( CMAKE_CXX_FLAGS_RELEASE  "-O2 -Wno-unused-result" )
ENDIF(MSVC)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmakefind/")

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

add_subdirectory( "querylib" )
add_subdirectory( "makedb" )
add_subdirectory( "cli" )
add_subdirectory( "scintilla/qt/ScintillaEdit" )
add_subdirectory( "showgraph" )
add_subdirectory( "gui" )



if (UNIX)

if (BUILD_QT5)

set( CPACK_PACKAGE_NAME "codequery-qt5" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "qtbase5-dev (>= 5.4.0) , libsqlite3-0 (>= 3.7.0) " )

else (BUILD_QT5)

set( CPACK_PACKAGE_NAME "codequery-qt4" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libqtcore4 (>= 4.8.0), libqtgui4 (>= 4.8.0) , libqt4-xml (>= 4.8.0) , libsqlite3-0 (>= 3.7.0) " )

endif (BUILD_QT5)


  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
  else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386" )
  endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

set( CPACK_GENERATOR "DEB;TGZ")
set( CPACK_PACKAGE_VERSION  "0.20.0" )
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

