

HEADERS+=\
    $$PWD/AppHelper.h\
    $$PWD/SkinFactory/SkinBlack.h \
    $$PWD/SkinFactory/SkinRed.h \
    $$PWD/SkinFactory/ISkin.h \
    $$PWD/SkinFactory/SkinFactory.h \
    $$PWD/SkinFactory/SkinPureColor.h \
    $$PWD/LyricSearcherFactory/LyricSearcherFactory.h \
    $$PWD/LyricSearcherFactory/SearcherNetEaseCloud.h \
    $$PWD/LyricSearcherFactory/SearcherGecimi.h \
    $$PWD/Phonograph.h \
    $$PWD/filter.h \
    $$PWD/ImageFilter.h \
    $$PWD/LyricViewer.h \
    $$PWD/LyricMaker.h \
    $$PWD/UnicodeReader.h\
    $$PWD/LyricSearcherFactory/ISearcher.h \
    $$PWD/LrcProcessor.h \
    $$PWD/LyricSearcherFactory/ThreadSearchLyric.h\
    $$PWD/LyricSearcherFactory/ThreadSearchNcmMusic.h\
    $$PWD/LyricListManager.h\
    $$PWD/SettingManager.h\
    $$PWD/ThreadGuessLyricInfo.h\
    $$PWD/MusicPlayerController.h\



SOURCES+=\
    $$PWD/LyricSearcherFactory/SearcherNetEaseCloud.cpp \
    $$PWD/LyricSearcherFactory/SearcherGecimi.cpp \
    $$PWD/AppHelper.cpp\
    $$PWD/Phonograph.cpp \
    $$PWD/LyricViewer.cpp \
    $$PWD/LyricMaker.cpp \
    $$PWD/LrcProcessor.cpp\
    $$PWD/LyricListManager.cpp\
    $$PWD/SettingManager.cpp\
    $$PWD/MusicPlayerController.cpp\




INCLUDEPATH +=$$PWD MusicPlayer
include(MusicPlayer/musicPlayer.pri)
