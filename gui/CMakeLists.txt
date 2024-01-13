#
# CodeQuery
# Copyright (C) 2013-2018 ruben2020 https://github.com/ruben2020/
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.5.0)
project(CodeQueryGUI)

#ADD_DEFINITIONS( -Wall )

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.10.0") 
foreach(p
    CMP0071 # 3.10: Let AUTOMOC and AUTOUIC process GENERATED files
    )
  if(POLICY ${p})
    cmake_policy(SET ${p} NEW)
  endif()
endforeach()
endif()



  # with SET() command you can change variables or define new ones
  # here we define CODEQUERY_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( CODEQUERY_SRCS
       ../querylib/small_lib.cpp
       ../querylib/sqlquery.cpp
       langtable.cpp
       fileviewer.cpp
       listhandler.cpp
       searchhandler.cpp
       mainwindow.cpp
       graphdialog.cpp
       aboutdialog.cpp
       fileviewsettingsdialog.cpp
       themes.cpp
       main_gui.cpp
       winmain.cpp
  )
  
  # another list, this time it includes all header files that should be treated with moc
  SET( CODEQUERY_MOC_HDRS
       fileviewer.h
       listhandler.h
       searchhandler.h
       mainwindow.h
       graphdialog.h
       aboutdialog.h
       fileviewsettingsdialog.h
  )
  
  # some .ui files
  SET( CODEQUERY_UIS
       ui/mainWindow.ui
       ui/graphDialog.ui
       ui/aboutDialog.ui
       ui/fileViewSettingsDialog.ui
  )
  
  # and finally an resource file
  SET( CODEQUERY_RCS
       codequery.qrc
  )

  # translation files
  SET( CODEQUERY_TRANS

translations/codequery_de.ts
translations/codequery_en.ts
translations/codequery_es.ts
translations/codequery_fr.ts
translations/codequery_id.ts
translations/codequery_it.ts
translations/codequery_ja.ts
translations/codequery_ko.ts
translations/codequery_zh-CHS.ts
translations/codequery_zh-CHT.ts

  )  

  SET( CQIMAGES_RC_QRC   "${CMAKE_CURRENT_SOURCE_DIR}/cqimages.qrc")
  SET( CQIMAGES_RC_SRCS  "${CMAKE_CURRENT_BINARY_DIR}/qrc_cqimages.cxx")
  SET( CQTRANS_RC_QRCT   "${CMAKE_CURRENT_SOURCE_DIR}/cqtrans.qrc.template")
  SET( CQTRANS_RC_QRC    "${CMAKE_CURRENT_BINARY_DIR}/cqtrans.qrc")
  SET( CQTRANS_RC_SRCS   "${CMAKE_CURRENT_BINARY_DIR}/qrc_cqtrans.cxx")

if (WIN32)
  SET( COPY_COMMAND  "copy")
  string(REPLACE "/" "\\" CQTRANS_RC_QRCT_CP "${CQTRANS_RC_QRCT}")
  string(REPLACE "/" "\\" CQTRANS_RC_QRC_CP  "${CQTRANS_RC_QRC}")
else (WIN32)
  SET( COPY_COMMAND  "cp")
  SET( CQTRANS_RC_QRCT_CP  "${CQTRANS_RC_QRCT}")
  SET( CQTRANS_RC_QRC_CP   "${CQTRANS_RC_QRC}")
endif (WIN32)

  add_definitions( -DNO_QDESIGNER_WIDGET_EXPORT )

  INCLUDE_DIRECTORIES( "../scintilla/qt/ScintillaEdit" )
  INCLUDE_DIRECTORIES( "../scintilla/qt/ScintillaEditBase" )
  INCLUDE_DIRECTORIES( "../scintilla/include" )
  INCLUDE_DIRECTORIES( "../scintilla/src" )
  INCLUDE_DIRECTORIES( "../scintilla/lexlib" )
  INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_BINARY_DIR}" )
  INCLUDE_DIRECTORIES( "." )
  INCLUDE_DIRECTORIES( "./translations" )
  INCLUDE_DIRECTORIES( "../querylib" )
  INCLUDE_DIRECTORIES( "../makedb" )
  INCLUDE_DIRECTORIES( "../showgraph" )
  INCLUDE_DIRECTORIES( "/usr/local/include" )

if(GHAWIN)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
set( SQLITE_LIBRARIES "unofficial::sqlite3::sqlite3" )
else(GHAWIN)
find_package(Sqlite REQUIRED)
include_directories( "${SQLITE_INCLUDE_DIR}" )
endif(GHAWIN)


if (BUILD_QT5)

  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt5Widgets 5.2.1 REQUIRED )
  FIND_PACKAGE( Qt5Core REQUIRED )
  FIND_PACKAGE( Qt5Concurrent REQUIRED )
  FIND_PACKAGE( Qt5Xml REQUIRED )
  FIND_PACKAGE( Qt5LinguistTools REQUIRED )

  add_definitions( -DUSE_QT5 )

  set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
  
  SET(CMAKE_AUTOMOC ON)
  SET(CMAKE_INCLUDE_CURRENT_DIR ON)

  get_target_property(QT_RCC_EXECUTABLE Qt5::rcc LOCATION)

  QT5_ADD_TRANSLATION( QM ${CODEQUERY_TRANS} )  
  
  # this command will generate rules that will run rcc on all files from CODEQUERY_RCS
  # in result CQIMAGES_RC_SRCS variable will contain paths to files produced by rcc
  # QT4_ADD_RESOURCES( CQIMAGES_RC_SRCS ${CODEQUERY_RCS} )

  # Run the resource compiler (rcc_options should already be set).
  ADD_CUSTOM_COMMAND(
    OUTPUT ${CQIMAGES_RC_SRCS}
    COMMAND ${QT_RCC_EXECUTABLE}
    ARGS ${rcc_options} -name cqimages -o ${CQIMAGES_RC_SRCS} ${CQIMAGES_RC_QRC}
    )

  ADD_CUSTOM_COMMAND(
    OUTPUT ${CQTRANS_RC_SRCS}
    COMMAND ${QT_RCC_EXECUTABLE}
    ARGS ${rcc_options} -name cqtrans -o ${CQTRANS_RC_SRCS} ${CQTRANS_RC_QRC}
    DEPENDS ${QM} ${CQTRANS_RC_QRC}
    )

  ADD_CUSTOM_COMMAND(
    OUTPUT ${CQTRANS_RC_QRC}
    COMMAND ${COPY_COMMAND}
    ARGS ${CQTRANS_RC_QRCT_CP} ${CQTRANS_RC_QRC_CP}
    )
    

  # this will run uic on .ui files:
  QT5_WRAP_UI( CODEQUERY_UI_HDRS ${CODEQUERY_UIS} )

  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( "${Qt5Widgets_INCLUDE_DIRS}" )

if(WIN32)
  SET(CQ_WIN_RCS cqwin64.rc)
  add_executable( codequery WIN32 ${CODEQUERY_SRCS} ${CODEQUERY_MOC_SRCS} ${CQIMAGES_RC_SRCS} ${CQTRANS_RC_SRCS} ${CODEQUERY_UI_HDRS} ${QM} ${CQ_WIN_RCS} )
  target_link_libraries( codequery Qt5::Widgets Qt5::Concurrent Qt5::WinMain ${SQLITE_LIBRARIES} cqshowgraph-qt5 scintillaedit)
else()
  add_executable( codequery ${CODEQUERY_SRCS} ${CODEQUERY_MOC_SRCS} ${CQIMAGES_RC_SRCS} ${CQTRANS_RC_SRCS} ${CODEQUERY_UI_HDRS} ${QM} )
  target_link_libraries( codequery Qt5::Widgets Qt5::Concurrent ${SQLITE_LIBRARIES} cqshowgraph-qt5 scintillaedit)
endif()
  
  install(TARGETS codequery RUNTIME DESTINATION bin)


else (BUILD_QT5)

  add_definitions( -DUSE_QT4 )

  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
  #SET( QT_USE_QTXML TRUE )
  #SET( QT_USE_QTSQL TRUE )
  SET( QT_DONT_USE_QTGUI FALSE )
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt4 REQUIRED )
  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  INCLUDE( "${QT_USE_FILE}" )

  QT4_ADD_TRANSLATION( QM ${CODEQUERY_TRANS} )  
  
  # this command will generate rules that will run rcc on all files from CODEQUERY_RCS
  # in result CQIMAGES_RC_SRCS variable will contain paths to files produced by rcc
  # QT4_ADD_RESOURCES( CQIMAGES_RC_SRCS ${CODEQUERY_RCS} )

  # Run the resource compiler (rcc_options should already be set).
  ADD_CUSTOM_COMMAND(
    OUTPUT ${CQIMAGES_RC_SRCS}
    COMMAND ${QT_RCC_EXECUTABLE}
    ARGS ${rcc_options} -name cqimages -o ${CQIMAGES_RC_SRCS} ${CQIMAGES_RC_QRC}
    )

  ADD_CUSTOM_COMMAND(
    OUTPUT ${CQTRANS_RC_SRCS}
    COMMAND ${QT_RCC_EXECUTABLE}
    ARGS ${rcc_options} -name cqtrans -o ${CQTRANS_RC_SRCS} ${CQTRANS_RC_QRC}
    DEPENDS ${QM} ${CQTRANS_RC_QRC}
    )

  ADD_CUSTOM_COMMAND(
    OUTPUT ${CQTRANS_RC_QRC}
    COMMAND ${COPY_COMMAND}
    ARGS ${CQTRANS_RC_QRCT_CP} ${CQTRANS_RC_QRC_CP}
    )
    

  # this will run uic on .ui files:
  QT4_WRAP_UI( CODEQUERY_UI_HDRS ${CODEQUERY_UIS} )
  
  # and finally this will run moc:
  QT4_WRAP_CPP( CODEQUERY_MOC_SRCS ${CODEQUERY_MOC_HDRS} )


if(WIN32)
  SET(CQ_WIN_RCS cqwin32.rc)
  add_executable( codequery WIN32 ${CODEQUERY_SRCS} ${CODEQUERY_MOC_SRCS} ${CQIMAGES_RC_SRCS} ${CQTRANS_RC_SRCS} ${CODEQUERY_UI_HDRS} ${QM} ${CQ_WIN_RCS} )
else()
  add_executable( codequery ${CODEQUERY_SRCS} ${CODEQUERY_MOC_SRCS} ${CQIMAGES_RC_SRCS} ${CQTRANS_RC_SRCS} ${CODEQUERY_UI_HDRS} ${QM} )
endif()

  target_link_libraries( codequery ${QT_LIBRARIES} ${SQLITE_LIBRARIES} cqshowgraph scintillaedit)
  
  install(TARGETS codequery RUNTIME DESTINATION bin)

endif (BUILD_QT5)


