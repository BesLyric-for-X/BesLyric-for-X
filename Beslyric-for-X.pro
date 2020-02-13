#-------------------------------------------------
#
# Project created by QtCreator 2018-07-05T22:26:31
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Beslyric-for-X
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH +=$$PWD BesWidgets
include(BesWidgets/BesWidgets.pri)

INCLUDEPATH +=$$PWD StackFrame
include(StackFrame/StackFrame.pri)

INCLUDEPATH +=$$PWD TopWidgets
include(TopWidgets/TopWidgets.pri)

INCLUDEPATH +=$$PWD MiddleWidgets
include(MiddleWidgets/MiddleWidgets.pri)

INCLUDEPATH +=$$PWD BottomWidgets
include(BottomWidgets/BottomWidgets.pri)

INCLUDEPATH +=$$PWD Entities
include(Entities/Entities.pri)

INCLUDEPATH +=$$PWD Utility
include(Utility/Utility.pri)


SOURCES += main.cpp \
    StackFrame.cpp

HEADERS  += \
    Define/Define.h\
    Define/Enum.h\
    Define/Struct.h\
    Define/Static.h\
    global.h \
    StackFrame.h \
    MyApplication.h

DISTFILES += \
    BesLyric.rc \
    version.txt

RESOURCES += \
    resource.qrc


# windows icon and exe file infomation
win32{
RC_FILE = Beslyric.rc
}

# set icon under Mac Os
macx{
ICON = Beslyric.icns
}

# ubuntu icon recoginition
#  No test on other Linux distros!
# from: https://stackoverflow.com/questions/45329372/ubuntu-recognizes-executable-as-shared-library-and-wont-run-it-by-clicking
!macx:unix{
    QMAKE_LFLAGS *= -no-pie
}

#--------------------------------

#屏蔽 msvc 编译器对 rational.h 的 warning: C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819"

win32{

#根据开发者自己 ffmpeg 和 sdl 库路径，可对如下路径进行修改，不过建议 库安装在 C:/lib 下，
#   具体使用步骤，可参看项目： https://github.com/BensonLaur/beslyric-lib

#ffmpeg

FFMPEG_INCLUDE  =   C:/lib/beslyric-lib/win32/ffmpeg_4_0_1/include
FFMPEG_LIB      =   C:/lib/beslyric-lib/win32/ffmpeg_4_0_1/lib

#sdl

SDL_INCLUDE     =   C:/lib/beslyric-lib/SDL_2_0_3/include
SDL_LIB         =   C:/lib/beslyric-lib/SDL_2_0_3/lib


#other
#OTHER_INCLUDE   =   C:/lib/beslyric-lib/win32/include

INCLUDEPATH +=  $$FFMPEG_INCLUDE \
                $$SDL_INCLUDE \
                $$OTHER_INCLUDE \

LIBS += -L$$FFMPEG_LIB/ -lavcodec\
        -L$$FFMPEG_LIB/ -lavdevice \
        -L$$FFMPEG_LIB/ -lavfilter \
        -L$$FFMPEG_LIB/ -lavutil \
        -L$$FFMPEG_LIB/ -lavformat \
        -L$$FFMPEG_LIB/ -lpostproc \
        -L$$FFMPEG_LIB/ -lswresample \
        -L$$FFMPEG_LIB/ -lswscale \
        -L$$FFMPEG_LIB/ -lswresample \
#        -L$$SDL_LIB/ -lSDL2main  \
        -L$$SDL_LIB/ -lSDL2
}


unix:!macx{

#消除ffmpeg中对使用旧接口的警告
QMAKE_CXXFLAGS += -Wno-deprecated-declarations

#根据开发者自己 ffmpeg 和 sdl 库路径，可对如下路径进行修改，不过建议 库安装在 /usr/local/ 下，
#   具体使用步骤，可参看项目： https://github.com/BensonLaur/beslyric-lib

# ffmpeg
FFMPEG_INCLUDE  = /usr/local/include
FFMPEG_LIB      = /usr/local/lib

#sdl
SDL_INCLUDE     = /usr/local/beslyric-lib/SDL_2_0_3/include
SDL_LIB         = /usr/local/beslyric-lib/SDL_2_0_3/lib

INCLUDEPATH +=  $$FFMPEG_INCLUDE \
                $$SDL_INCLUDE \

LIBS += $$FFMPEG_LIB/libavcodec.so      \
        $$FFMPEG_LIB/libavdevice.so     \
        $$FFMPEG_LIB/libavfilter.so     \
        $$FFMPEG_LIB/libavformat.so     \
        $$FFMPEG_LIB/libavutil.so       \
        $$FFMPEG_LIB/libswresample.so   \
        $$FFMPEG_LIB/libswscale.so      \
        #$$FFMPEG_LIB/libpostproc.so    \
#        -L$$SDL_LIB/ -lSDL2main        \   #can't be $$PSDL_LIB/ -lSDL2main, must be -L$$PSDL_LIB/ -lSDL2main
        -L$$SDL_LIB/ -lSDL2
}


macx{
#根据开发者自己 ffmpeg 和 sdl 库路径，可对如下路径进行修改，不过建议 库安装在 /usr/local/ 下，
#   具体使用步骤，可参看项目： https://github.com/BensonLaur/beslyic-lib

# ffmpeg
FFMPEG_INCLUDE  = /usr/local/include
FFMPEG_LIB      = /usr/local/lib

#sdl
SDL_INCLUDE     = /usr/local/include/SDL2
SDL_LIB         = /usr/local/lib

INCLUDEPATH +=  $$FFMPEG_INCLUDE \
                $$SDL_INCLUDE \

LIBS += -L$$FFMPEG_LIB/ -lavcodec      \
        -L$$FFMPEG_LIB/ -lavdevice     \
        -L$$FFMPEG_LIB/ -lavfilter     \
        -L$$FFMPEG_LIB/ -lavformat     \
        -L$$FFMPEG_LIB/ -lavutil       \
        -L$$FFMPEG_LIB/ -lswresample   \
        -L$$FFMPEG_LIB/ -lswscale   \
#        -L$$SDL_LIB/ -lSDL2main        \
        -L$$SDL_LIB/ -lSDL2

}

