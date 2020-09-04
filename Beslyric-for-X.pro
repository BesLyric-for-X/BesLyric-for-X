#-------------------------------------------------
#
# Project created by QtCreator 2018-07-05T22:26:31
#
#-------------------------------------------------

QT       += core gui network widgets


# For OpenSSL 1.1.1, we need Qt 5.12.4+.
#   https://www.qt.io/blog/2019/06/17/qt-5-12-4-released-support-openssl-1-1-1
#   https://github.com/BesLyric-for-X/BesLyric-for-X/issues/29

lessThan(QT_MAJOR_VERSION, 5) {
    error("This app need Qt version >= 5.12.4, you are using $$[QT_VERSION].")
}
else:equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 12) {
        error("This app need Qt version >= 5.12.4, you are using $$[QT_VERSION].")
    }
    else:equals(QT_MINOR_VERSION, 12):lessThan(QT_PATCH_VERSION, 4) {
        error("This app need Qt version >= 5.12.4, you are using $$[QT_VERSION].")
    }
}


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

# To enable qDebug() under "release", disable QT_NO_DEBUG_OUTPUT bellow
CONFIG (release, debug|release) {
DEFINES *= QT_NO_DEBUG_OUTPUT
}

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

# Separate the binary file.
CONFIG(debug, debug|release){
    DESTDIR = $${OUT_PWD}/debug_bin
}
CONFIG(release, debug|release){
    DESTDIR = $${OUT_PWD}/release_bin
}

#--------------------------------

#屏蔽 msvc 编译器对 rational.h 的 warning: C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819"


unix:!macx{

#消除ffmpeg中对使用旧接口的警告
QMAKE_CXXFLAGS += -Wno-deprecated-declarations

}

#--------------------------------

# Dependencies

include(.DependencyConfig/.DependencyConfig.pri)

#--------------------------------
