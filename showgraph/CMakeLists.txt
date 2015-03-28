cmake_minimum_required(VERSION 2.8.12)
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
   
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt5Widgets 5.3.0 REQUIRED )
  FIND_PACKAGE( Qt5Concurrent REQUIRED )
  FIND_PACKAGE( Qt5Xml REQUIRED )
  SET(CMAKE_AUTOMOC ON)
  SET(CMAKE_INCLUDE_CURRENT_DIR ON)
  
  # this command will generate rules that will run rcc on all files from CODEQUERY_RCS
  # in result CODEQUERY_RC_SRCS variable will contain paths to files produced by rcc
  # QT4_ADD_RESOURCES( CODEQUERY_RC_SRCS ${CODEQUERY_RCS} )


  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( "${Qt5Widgets_INCLUDE_DIRS}" )
  INCLUDE_DIRECTORIES( "${Qt5Xml_INCLUDE_DIRS}" )
  INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_BINARY_DIR}" )
  INCLUDE_DIRECTORIES( "." )
  INCLUDE_DIRECTORIES( "./Graph" )
  INCLUDE_DIRECTORIES( "./GraphView" )
  INCLUDE_DIRECTORIES( "./Layout" )
  INCLUDE_DIRECTORIES( "./Utils" )

  add_library( cqshowgraph STATIC ${SHOWGRAPH_SRCS} ${SHOWGRAPH_MOC_SRCS} )
  target_link_libraries( cqshowgraph Qt5::Widgets Qt5::Concurrent Qt5::Xml )

