cmake_minimum_required(VERSION 3.16.0)
project(ShowGraph)

#ADD_DEFINITIONS( -Wall )


  # with SET() command you can change variables or define new ones
  # here we define SHOWGRAPH_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( SHOWGRAPH_SRCS
./showgraph.cpp
./Utils/mem_mgr.cpp
./Utils/mem_utest.cpp
./Utils/conf.cpp
./Utils/list_utest.cpp
./Utils/utils_utest.cpp
./Utils/utils.cpp
./Utils/conf_utest.cpp
./Utils/mem_pool.cpp
./Layout/node_group.cpp
./Layout/aux_graph.cpp
./Layout/layout.cpp
./Graph/graph.cpp
./Graph/node.cpp
./Graph/edge.cpp
./GraphView/edge_item.cpp
./GraphView/style_edit.cpp
./GraphView/graph_view.cpp
./GraphView/visible_edge.cpp
./GraphView/node_item.cpp
./GraphView/edge_helper.cpp
./GraphView/navigation.cpp
  )
  
  # another list, this time it includes all header files that should be treated with moc
  SET( SHOWGRAPH_MOC_HDRS
./Layout/aux_graph.h
./GraphView/graph_view.h
./GraphView/style_edit.h
  )

  INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_BINARY_DIR}" )
  INCLUDE_DIRECTORIES( "." )
  INCLUDE_DIRECTORIES( "./Graph" )
  INCLUDE_DIRECTORIES( "./GraphView" )
  INCLUDE_DIRECTORIES( "./Layout" )
  INCLUDE_DIRECTORIES( "./Utils" )
  
if (BUILD_QT6)

  find_package(Qt6 REQUIRED COMPONENTS Core Widgets Concurrent Xml Core5Compat)
  qt6_standard_project_setup()

  add_definitions( -DUSE_QT5 )

  set_target_properties(Qt6::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

  qt6_add_library( cqshowgraph-qt6 STATIC ${SHOWGRAPH_SRCS} ${SHOWGRAPH_MOC_SRCS} )
  target_link_libraries( cqshowgraph-qt6 PRIVATE Qt6::Widgets Qt6::Concurrent Qt6::Xml Qt6::Core5Compat )

else (BUILD_QT6)

if (BUILD_QT5)


  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt5Widgets 5.2.1 REQUIRED )
  FIND_PACKAGE( Qt5Concurrent REQUIRED )
  FIND_PACKAGE( Qt5Xml REQUIRED )

  add_definitions( -DUSE_QT5 )
  
  set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
  #add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x050F00)

  SET(CMAKE_AUTOMOC ON)
  SET(CMAKE_INCLUDE_CURRENT_DIR ON)
  
  # this command will generate rules that will run rcc on all files from CODEQUERY_RCS
  # in result CODEQUERY_RC_SRCS variable will contain paths to files produced by rcc
  # QT4_ADD_RESOURCES( CODEQUERY_RC_SRCS ${CODEQUERY_RCS} )


  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( "${Qt5Widgets_INCLUDE_DIRS}" )
  INCLUDE_DIRECTORIES( "${Qt5Xml_INCLUDE_DIRS}" )


  add_library( cqshowgraph-qt5 STATIC ${SHOWGRAPH_SRCS} ${SHOWGRAPH_MOC_SRCS} )
  target_link_libraries( cqshowgraph-qt5 Qt5::Widgets Qt5::Concurrent Qt5::Xml )


else (BUILD_QT5)

  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
  SET( QT_USE_QTXML TRUE )
  #SET( QT_USE_QTSQL TRUE )
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt4 REQUIRED )
  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  INCLUDE( "${QT_USE_FILE}" )
  
  # this command will generate rules that will run rcc on all files from CODEQUERY_RCS
  # in result CODEQUERY_RC_SRCS variable will contain paths to files produced by rcc
  # QT4_ADD_RESOURCES( CODEQUERY_RC_SRCS ${CODEQUERY_RCS} )

  # and finally this will run moc:
  QT4_WRAP_CPP( SHOWGRAPH_MOC_SRCS ${SHOWGRAPH_MOC_HDRS} )

  add_library( cqshowgraph STATIC ${SHOWGRAPH_SRCS} ${SHOWGRAPH_MOC_SRCS} )
  target_link_libraries( cqshowgraph ${QT_LIBRARIES} )

endif (BUILD_QT5)
endif (BUILD_QT6)

