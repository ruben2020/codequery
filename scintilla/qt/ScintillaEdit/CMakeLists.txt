cmake_minimum_required(VERSION 3.5.0)
project(ScintillaEdit)


  SET(SCINTILLA_SRCS
    ScintillaEdit.cpp
    ScintillaDocument.cpp
    ../ScintillaEditBase/PlatQt.cpp
    ../ScintillaEditBase/ScintillaQt.cpp
    ../ScintillaEditBase/ScintillaEditBase.cpp
      )

  
  SET( SCINTILLA_MOC_HDRS
    ScintillaEdit.h
    ScintillaDocument.h
    ../ScintillaEditBase/ScintillaEditBase.h
    ../ScintillaEditBase/ScintillaQt.h
  )
  
  FILE(GLOB LEXER_SRCS  "../../lexers/*.cxx")
  FILE(GLOB LEXLIB_SRCS "../../lexlib/*.cxx")
  FILE(GLOB SCINT_SRCS  "../../src/*.cxx")

  INCLUDE_DIRECTORIES( "../ScintillaEditBase" )
  INCLUDE_DIRECTORIES( "../../include" )
  INCLUDE_DIRECTORIES( "../../src" )
  INCLUDE_DIRECTORIES( "../../lexlib" )
  INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_BINARY_DIR}" )
  INCLUDE_DIRECTORIES( "." )
  INCLUDE_DIRECTORIES( "/usr/local/include" )

  add_definitions( -DSCINTILLA_QT=1 -DSCI_LEXER=1 -D_CRT_SECURE_NO_DEPRECATE=1 -DSTATIC_BUILD=1 )


if (BUILD_QT5)

  FIND_PACKAGE( Qt5Widgets 5.2.1 REQUIRED )
  FIND_PACKAGE( Qt5Core REQUIRED )
  FIND_PACKAGE( Qt5Concurrent REQUIRED )

  set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
  
  SET(CMAKE_AUTOMOC ON)
  SET(CMAKE_INCLUDE_CURRENT_DIR ON)

  INCLUDE_DIRECTORIES( "${Qt5Widgets_INCLUDE_DIRS}" )

  add_library( scintillaedit STATIC ${SCINTILLA_SRCS} ${SCINTILLA_MOC_SRCS} ${LEXER_SRCS} ${LEXLIB_SRCS} ${SCINT_SRCS} )
  target_link_libraries( scintillaedit Qt5::Widgets Qt5::Concurrent )

else (BUILD_QT5)
 
  SET( QT_DONT_USE_QTGUI FALSE )
  
  FIND_PACKAGE( Qt4 REQUIRED )
  
  INCLUDE( "${QT_USE_FILE}" )

  QT4_WRAP_CPP( SCINTILLA_MOC_SRCS ${SCINTILLA_MOC_HDRS} )


  add_library( scintillaedit STATIC ${SCINTILLA_SRCS} ${SCINTILLA_MOC_SRCS} ${LEXER_SRCS} ${LEXLIB_SRCS} ${SCINT_SRCS} )
  target_link_libraries( scintillaedit ${QT_LIBRARIES} )

endif (BUILD_QT5)

