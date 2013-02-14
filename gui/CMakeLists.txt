cmake_minimum_required(VERSION 2.8)
project(CodeQueryGUI)

#ADD_DEFINITIONS( -Wall )

  # with SET() command you can change variables or define new ones
  # here we define CODEQUERY_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( CODEQUERY_SRCS
       CodeEditor.cpp
       highlighter.cpp
       sqlquery.cpp
       fileviewer.cpp
       listhandler.cpp
       searchhandler.cpp
       mainwindow.cpp
       main_gui.cpp
  )
  
  # another list, this time it includes all header files that should be treated with moc
  SET( CODEQUERY_MOC_HDRS
       CodeEditor.h
       highlighter.h
       fileviewer.h
       listhandler.h
       searchhandler.h
       mainwindow.h
  )
  
  # some .ui files
  SET( CODEQUERY_UIS
       ui/mainWindow.ui
  )
  
  # and finally an resource file
  SET( CODEQUERY_RCS
       codequery.qrc
  )

  # translation files
  SET( CODEQUERY_TRANS
       translations/codequery_en.ts
       translations/codequery_de.ts
       translations/codequery_es.ts
       translations/codequery_fr.ts
       translations/codequery_id.ts
  )  

  add_definitions( -DNO_QDESIGNER_WIDGET_EXPORT )
  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
  #SET( QT_USE_QTXML TRUE )
  #SET( QT_USE_QTSQL TRUE )
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt4 REQUIRED )
  FIND_PACKAGE( Sqlite REQUIRED)
  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  INCLUDE( ${QT_USE_FILE} )

  QT4_ADD_TRANSLATION( QM ${CODEQUERY_TRANS} )  
  
  # this command will generate rules that will run rcc on all files from CODEQUERY_RCS
  # in result CODEQUERY_RC_SRCS variable will contain paths to files produced by rcc
  # QT4_ADD_RESOURCES( CODEQUERY_RC_SRCS ${CODEQUERY_RCS} )


  SET( CODEQUERY_RC_NAME codequery )
  SET( CODEQUERY_RC_QRC ${CMAKE_CURRENT_SOURCE_DIR}/${CODEQUERY_RC_NAME}.qrc)
  SET( CODEQUERY_RC_SRCS ${CMAKE_CURRENT_BINARY_DIR}/qrc_${CODEQUERY_RC_NAME}.cxx)

  # Run the resource compiler (rcc_options should already be set). We can't
  # use QT4_ADD_RESOURCES because the qrc file may not exist yet.
  ADD_CUSTOM_COMMAND(
    OUTPUT ${CODEQUERY_RC_SRCS}
    COMMAND ${QT_RCC_EXECUTABLE}
    ARGS ${rcc_options} -name ${CODEQUERY_RC_NAME} -o ${CODEQUERY_RC_SRCS} ${CODEQUERY_RC_QRC}
    DEPENDS ${QM}
    )

  # this will run uic on .ui files:
  QT4_WRAP_UI( CODEQUERY_UI_HDRS ${CODEQUERY_UIS} )
  
  # and finally this will run moc:
  QT4_WRAP_CPP( CODEQUERY_MOC_SRCS ${CODEQUERY_MOC_HDRS} )

  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( ${SQLITE_INCLUDE_DIR} )
  INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR}/gui )
  INCLUDE_DIRECTORIES( "." )
  INCLUDE_DIRECTORIES( "../makedb" )

  add_executable( codequery ${CODEQUERY_SRCS} ${CODEQUERY_MOC_SRCS} ${CODEQUERY_RC_SRCS} ${CODEQUERY_UI_HDRS} ${QM} )
  target_link_libraries( codequery ${QT_LIBRARIES} ${SQLITE_LIBRARIES} small_lib)
  
