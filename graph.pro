#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T17:25:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graph
TEMPLATE = app

CONFIG += c++11

#LIBS += lpthread

# for gprof profilers
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg


SOURCES += gui/main.cpp\
    graph/Edge.cpp \
    graph/FileTypes.cpp \
    graph/Graph.cpp \
    graph/GraphBase.cpp \
    graph/GraphGenerator.cpp \
    graph/GraphReader.cpp \
    graph/GraphWriter.cpp \
    graph/Node.cpp \
    graph/ShortPath.cpp \
    graph/ShortPathElem.cpp \
    gui/OpenFileForm.cpp \
    gui/MainWindow.cpp \
    gui/GraphGeneratorForm.cpp \
    graph/ReaderBase.cpp \
    graph/ShortPathReader.cpp \
    graph/WriterBase.cpp \
    graph/ShortPathWriter.cpp \
    gui/SaveForm.cpp \
    gui/SaveGraphForm.cpp \
    gui/SaveShortPathForm.cpp \
    gui/GraphInfoForm.cpp \
    graph/ShortPathRootElem.cpp \
    gui/AboutForm.cpp \
    graph/Worker.cpp \
    gui/GraphWorker.cpp \
    graph/ProjectionElem.cpp \
    graph/Projection.cpp \
    graph/Projections.cpp \
    graph/ProjectionsWriter.cpp \
    graph/ProjectionsReader.cpp \
    gui/NodeInfoForm.cpp \
    gui/StatisticMenuForm.cpp

HEADERS  += \
    graph/Edge.h \
    graph/FileTypes.h \
    graph/Graph.h \
    graph/GraphBase.h \
    graph/GraphGenerator.h \
    graph/GraphReader.h \
    graph/GraphWriter.h \
    graph/Node.h \
    graph/ShortPath.h \
    graph/ShortPathElem.h \
    gui/OpenFileForm.h \
    gui/MainWindow.h \
    gui/GraphGeneratorForm.h \
    graph/ReaderBase.h \
    graph/ShortPathReader.h \
    graph/WriterBase.h \
    graph/ShortPathWriter.h \
    gui/SaveForm.h \
    gui/SaveGraphForm.h \
    gui/SaveShortPathForm.h \
    gui/GraphInfoForm.h \
    graph/ShortPathRootElem.h \
    graph/types.h \
    gui/AboutForm.h \
    graph/Worker.h \
    gui/GraphWorker.h \
    graph/ProjectionElem.h \
    graph/Projection.h \
    graph/Projections.h \
    graph/ProjectionsWriter.h \
    graph/ProjectionsReader.h \
    gui/NodeInfoForm.h \
    gui/StatisticMenuForm.h

FORMS    += \
    gui/OpenFileForm.ui \
    gui/MainWindow.ui \
    gui/GraphGeneratorForm.ui \
    gui/SaveForm.ui \
    gui/SaveGraphForm.ui \
    gui/SaveShortPathForm.ui \
    gui/GraphInfoForm.ui \
    gui/AboutForm.ui \
    gui/NodeInfoForm.ui \
    gui/StatisticMenuForm.ui

DISTFILES += \
    gui/resources/translations/graph_ru.ts


TRANSLATIONS += \
    gui/resources/translations/graph_ru.ts

RESOURCES += \
    gui/resources/resources.qrc
