

HEADERS+=\
    $$PWD/AppHelper.h\
    $$PWD/SkinFactory/SkinBlack.h \
    $$PWD/SkinFactory/SkinRed.h \
    $$PWD/SkinFactory/ISkin.h \
    $$PWD/SkinFactory/SkinFactory.h \
    $$PWD/SkinFactory/SkinPureColor.h \
    $$PWD/Phonograph.h \
    $$PWD/filter.h \
    $$PWD/ImageFilter.h \
    $$PWD/LyricViewer.h \
    $$PWD/LyricMaker.h


SOURCES+=\
    $$PWD/AppHelper.cpp\
    $$PWD/Phonograph.cpp \
    $$PWD/LyricViewer.cpp \
    $$PWD/LyricMaker.cpp


INCLUDEPATH +=$$PWD MusicPlayer
include(MusicPlayer/musicPlayer.pri)
