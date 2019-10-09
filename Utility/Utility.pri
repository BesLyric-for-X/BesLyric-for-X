
QT       += network

HEADERS+=\
    $$PWD/NetAccess.h


SOURCES+=\
    $$PWD/NetAccess.cpp

#win32-g++*:
#win32-msvc*:

#mac下只需要 QT += network 即可，linux 和 windows 才需要明确指定 Qt5NetWork
!macx{
#    CONFIG(debug, debug|release):LIBS += -lQt5Networkd
#    CONFIG(release, debug|release):LIBS += -lQt5Network
}
