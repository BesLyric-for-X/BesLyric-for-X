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
# 注意 musicPlayer.pri 中关于 INSTALLS 的配置也与这个有关

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


# WIN32_LIB_PATH 位于 musicPlayer.pri 中，以后可以放在独立的 .pri 文件中

# OpenSSL library
## openssl 1.0.x (binary only)

    OPENSSL_BIN     =   $$WIN32_LIB_PATH/OpenSSL/i386/1.0.2t

    libs_openssl_related.files = $$OPENSSL_BIN/libeay32.dll \
                   $$OPENSSL_BIN/ssleay32.dll

    #.pro 对 CONFIG 做了调整，不再有 debug 和 release 目录分别在 debug 和 release configuration 下生成
    CONFIG(debug, debug|release):libs_openssl_related.path = $$OUT_PWD/debug_output
    CONFIG(release, debug|release):libs_openssl_related.path = $$OUT_PWD/release_output

    INSTALLS += libs_openssl_related

# Universal C RunTime library
## ucrt 10.0.14393.795

    UCRT_BIN     =   $$WIN32_LIB_PATH/UCRT/x86/10.0.14393.795

    libs_ucrt_related.files = $$UCRT_BIN/api-ms-win-core-console-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-datetime-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-debug-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-errorhandling-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-file-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-file-l1-2-0.dll \
                   $$UCRT_BIN/api-ms-win-core-file-l2-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-handle-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-heap-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-interlocked-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-libraryloader-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-localization-l1-2-0.dll \
                   $$UCRT_BIN/api-ms-win-core-memory-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-namedpipe-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-processenvironment-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-processthreads-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-processthreads-l1-1-1.dll \
                   $$UCRT_BIN/api-ms-win-core-profile-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-rtlsupport-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-string-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-synch-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-synch-l1-2-0.dll \
                   $$UCRT_BIN/api-ms-win-core-sysinfo-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-timezone-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-core-util-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-conio-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-convert-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-environment-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-filesystem-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-heap-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-locale-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-math-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-multibyte-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-private-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-process-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-runtime-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-stdio-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-string-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-time-l1-1-0.dll \
                   $$UCRT_BIN/api-ms-win-crt-utility-l1-1-0.dll \
                   $$UCRT_BIN/ucrtbase.dll

    #.pro 对 CONFIG 做了调整，不再有 debug 和 release 目录分别在 debug 和 release configuration 下生成
    CONFIG(debug, debug|release):libs_ucrt_related.path = $$OUT_PWD/debug_output
    CONFIG(release, debug|release):libs_ucrt_related.path = $$OUT_PWD/release_output

    INSTALLS += libs_ucrt_related

# Microsoft C RunTime library
## crt 14.0.24234.1

    CRT_BIN     =   $$WIN32_LIB_PATH/CRT/x86/14.0.24234.1

    libs_crt_related.files = $$CRT_BIN/concrt140.dll \
                   $$CRT_BIN/msvcp140.dll \
                   $$CRT_BIN/vcruntime140.dll

    #.pro 对 CONFIG 做了调整，不再有 debug 和 release 目录分别在 debug 和 release configuration 下生成
    CONFIG(debug, debug|release):libs_crt_related.path = $$OUT_PWD/debug_output
    CONFIG(release, debug|release):libs_crt_related.path = $$OUT_PWD/release_output

    INSTALLS += libs_crt_related

#--
message(INSTALLS: $$INSTALLS)
}

