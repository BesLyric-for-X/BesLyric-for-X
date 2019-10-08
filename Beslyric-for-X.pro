#-------------------------------------------------
#
# Project created by QtCreator 2018-07-05T22:26:31
#
#-------------------------------------------------

QT       += core gui

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

# https://stackoverflow.com/questions/3440387/how-to-put-generated-files-e-g-object-files-into-a-separate-folder-when-using
# 中间文件都放在 <conf>_generated 下，生成的二进制文件放在 <conf>_output 下

CONFIG(debug, debug|release) {
    OBJECTS_DIR = $$OUT_PWD/debug_generated
    MOC_DIR = $$OUT_PWD/debug_generated
    RCC_DIR = $$OUT_PWD/debug_generated

    DESTDIR = $$OUT_PWD/debug_output
}
CONFIG(release, debug|release) {
    OBJECTS_DIR = $$OUT_PWD/release_generated
    MOC_DIR = $$OUT_PWD/release_generated
    RCC_DIR = $$OUT_PWD/release_generated

    DESTDIR = $$OUT_PWD/release_output
}

win32 {

# https://stackoverflow.com/questions/28993418/qt-creator-creates-both-a-debug-and-a-release-folder-inside-the-designated-debug
# https://stackoverflow.com/questions/1298988/qt-does-not-create-output-files-in-debug-release-folders-in-linux

    message($$CONFIG)
    CONFIG -= debug_and_release debug_and_release_target

}

