
QT       += network

HEADERS+=\
    $$PWD/NetAccess.h


SOURCES+=\
    $$PWD/NetAccess.cpp

#win32-g++*:
#win32-msvc*:

#后来在 macOs 10.12 / ubuntu 18.04 / windows 上测试发现都不需要显示指定 Qt5NetWork
#mac下只需要 QT += network 即可，linux 和 windows 才需要明确指定 Qt5NetWork
!macx{
#    CONFIG(debug, debug|release):LIBS += -lQt5Networkd
#    CONFIG(release, debug|release):LIBS += -lQt5Network
}
