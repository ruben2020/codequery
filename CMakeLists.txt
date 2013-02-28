cmake_minimum_required(VERSION 2.8)
project(CodeQuery)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/helper/")

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

INCLUDE_DIRECTORIES( "../makedb" )
set( OPTLISTLIB_SRCS makedb/optlist.cpp )
add_library( optlist_lib STATIC ${OPTLISTLIB_SRCS} )

add_subdirectory( "querylib" )
add_subdirectory( "makedb" )
add_subdirectory( "cli" )
add_subdirectory( "showgraph" )
add_subdirectory( "gui" )

if (UNIX)

set( CPACK_GENERATOR "DEB;TGZ")
set( CPACK_PACKAGE_NAME "codequery" )
set( CPACK_PACKAGE_VERSION  "0.06" )
set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libqtgui4 (>= 4.7.0) , libqt4-xml (>= 4.7.0) , libsqlite3-0 (>= 3.7.0) , cscope , exuberant-ctags")
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Code understanding and searching tool")
set( CPACK_PACKAGE_DESCRIPTION "Code understanding and searching tool")
set( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
set( CPACK_PACKAGE_CONTACT "ruben2020 ruben2020-git@users.sourceforge.net")

include(CPack)

endif(UNIX)

