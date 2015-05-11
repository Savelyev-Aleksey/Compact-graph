TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Edge.cpp \
    Graph.cpp \
    GraphBase.cpp \
    Node.cpp \
    ShortPath.cpp \
    ShortPathElem.cpp \
    GraphGenerator.cpp \
    GraphReader.cpp \
    GraphWriter.cpp \
    FileTypes.cpp


HEADERS += \
    Edge.h \
    Graph.h \
    GraphBase.h \
    Node.h \
    ShortPath.h \
    ShortPathElem.h \
    GraphGenerator.h \
    GraphReader.h \
    GraphWriter.h \
    FileTypes.h

