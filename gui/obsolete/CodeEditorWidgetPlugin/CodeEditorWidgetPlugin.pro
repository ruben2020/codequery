 CONFIG      += designer plugin
 TARGET      = $$qtLibraryTarget($$TARGET)
 TEMPLATE    = lib
 QTDIR_build:DESTDIR     = $$QT_BUILD_TREE/plugins/designer
 INCLUDEPATH += ..
 #OBJECTS_DIR = build
 
 HEADERS     = ../CodeEditor.h \
               CodeEditorWidgetPlugin.h
 SOURCES     = ../CodeEditor.cpp \
               CodeEditorWidgetPlugin.cpp

 # install
 target.path = $$[QT_INSTALL_PLUGINS]/designer
 sources.files = $$SOURCES $$HEADERS *.pro
 sources.path = $$[QT_INSTALL_EXAMPLES]/designer/CodeEditorWidgetPlugin
 INSTALLS += target sources


