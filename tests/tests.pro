QT       += testlib qml

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++14

TEMPLATE = app

SOURCES +=     main.cpp \    
    testcases.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += QUICK_TEST_SOURCE_DIR=\\\"$$PWD/qmltests\\\"
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

ROOT_DIR = $$absolute_path($$PWD/..)
INCLUDEPATH += $$ROOT_DIR/src/cpp

include(qpm.pri)

DISTFILES +=     qpm.json     \
    ../.travis.yml \
    ../README.md \
    ../appveyor.yml \
    qpm.pri

!win32 {
    QMAKE_CXXFLAGS += -Werror
}

HEADERS += \
    testcases.h

