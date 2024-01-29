cmake_minimum_required(VERSION 3.16.0)
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

  FILE(GLOB SCINT_SRCS  "../../src/*.cxx")


  INCLUDE_DIRECTORIES( "../ScintillaEditBase" )
  INCLUDE_DIRECTORIES( "../../include" )
  INCLUDE_DIRECTORIES( "../../src" )
  INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_BINARY_DIR}" )
  INCLUDE_DIRECTORIES( "." )
  INCLUDE_DIRECTORIES( "/usr/local/include" )

  add_definitions( -DSCINTILLA_QT=1 -DSCI_LEXER=1 -D_CRT_SECURE_NO_DEPRECATE=1 -DSTATIC_BUILD=1 )

if (BUILD_QT6)

  find_package(Qt6 REQUIRED COMPONENTS Core Widgets Concurrent Core5Compat )
	find_package( Threads REQUIRED )
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  set(CMAKE_AUTOUIC ON)

  set_target_properties(Qt6::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

  qt6_add_library( scintillaedit STATIC ${SCINTILLA_SRCS} ${SCINTILLA_MOC_SRCS} ${SCINT_SRCS} )
  target_link_libraries( scintillaedit PRIVATE Qt6::Widgets Qt6::Concurrent Qt6::Core5Compat Threads::Threads )

else (BUILD_QT6)
if (BUILD_QT5)

  FIND_PACKAGE( Qt5Widgets REQUIRED )
  FIND_PACKAGE( Qt5Core REQUIRED )
  FIND_PACKAGE( Qt5Concurrent REQUIRED )
  FIND_PACKAGE( Threads REQUIRED )
	
  set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
# add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x050F00)
  
  SET(CMAKE_AUTOMOC ON)
  SET(CMAKE_INCLUDE_CURRENT_DIR ON)

  INCLUDE_DIRECTORIES( "${Qt5Widgets_INCLUDE_DIRS}" )

  add_library( scintillaedit STATIC ${SCINTILLA_SRCS} ${SCINTILLA_MOC_SRCS} ${SCINT_SRCS} )
  target_link_libraries( scintillaedit Qt5::Widgets Qt5::Concurrent Threads::Threads )

endif (BUILD_QT5)
endif (BUILD_QT6)

