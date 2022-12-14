GOOGLETEST_DIR = /usr/src/googletest
include(gtest_dependency.pri)

QT += core gui

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread

QMAKE_CXXFLAGS += -Wall -Wextra
QMAKE_CFLAGS += -Wall -Wextra -Werror

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_CFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

HEADERS += ../app/models/devicesmodel.h

SOURCES += main.cpp \
    ../app/entities/device.cpp \
    ../app/models/devicesmodel.cpp \
    devicesmodel_tests.cpp

INCLUDEPATH += ../app
