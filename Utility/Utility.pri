

HEADERS+=\
    $$PWD/NetAccess.h


SOURCES+=\
    $$PWD/NetAccess.cpp

CONFIG(debug, debug|release):LIBS += Qt5Networkd.lib
CONFIG(release, debug|release):LIBS += Qt5Network.lib

