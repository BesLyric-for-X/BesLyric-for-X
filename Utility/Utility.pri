
QT       += network

HEADERS+=\
    $$PWD/NetAccess.h


SOURCES+=\
    $$PWD/NetAccess.cpp

#win32-g++*:
#win32-msvc*:

CONFIG(debug, debug|release):LIBS += -lQt5Networkd
CONFIG(release, debug|release):LIBS += -lQt5Network

