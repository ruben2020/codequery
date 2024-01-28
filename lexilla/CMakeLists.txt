cmake_minimum_required(VERSION 3.16.0)
project(Lexilla)
  
FILE(GLOB LEXER_SRCS  "./lexers/*.cxx")
FILE(GLOB LEXLIB_SRCS "./lexlib/*.cxx")
FILE(GLOB LEXINT_SRCS  "./src/*.cxx")

INCLUDE_DIRECTORIES( "./include" )
INCLUDE_DIRECTORIES( "./src" )
INCLUDE_DIRECTORIES( "./lexlib" )
INCLUDE_DIRECTORIES( "../scintilla/include" )

add_definitions( -DSCINTILLA_QT=1 -DSCI_LEXER=1 -D_CRT_SECURE_NO_DEPRECATE=1 -DSTATIC_BUILD=1 )

add_library( lexilla STATIC ${LEXER_SRCS} ${LEXLIB_SRCS} ${LEXINT_SRCS} )
target_link_libraries( lexilla )

