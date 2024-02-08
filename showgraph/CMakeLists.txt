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

  find_package(Qt6 REQUIRED COMPONENTS Core Widgets Concurrent Xml )
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  set(CMAKE_AUTOUIC ON)
  
  add_definitions( -DUSE_QT5 )
  SET(CMAKE_INCLUDE_CURRENT_DIR ON)

  set_target_properties(Qt6::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

  qt6_add_library( cqshowgraph-qt6 STATIC ${SHOWGRAPH_SRCS} ${SHOWGRAPH_MOC_SRCS} )
  target_link_libraries( cqshowgraph-qt6 PRIVATE Qt6::Widgets Qt6::Concurrent Qt6::Xml )

else (BUILD_QT6)

  FIND_PACKAGE( Qt5Widgets REQUIRED )
  FIND_PACKAGE( Qt5Concurrent REQUIRED )
  FIND_PACKAGE( Qt5Xml REQUIRED )

  add_definitions( -DUSE_QT5 )
  
  set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
  #add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x050F00)

  SET(CMAKE_AUTOMOC ON)
  SET(CMAKE_INCLUDE_CURRENT_DIR ON)

  INCLUDE_DIRECTORIES( "${Qt5Widgets_INCLUDE_DIRS}" )
  INCLUDE_DIRECTORIES( "${Qt5Xml_INCLUDE_DIRS}" )

  add_library( cqshowgraph-qt5 STATIC ${SHOWGRAPH_SRCS} ${SHOWGRAPH_MOC_SRCS} )
  target_link_libraries( cqshowgraph-qt5 Qt5::Widgets Qt5::Concurrent Qt5::Xml )

endif (BUILD_QT6)

