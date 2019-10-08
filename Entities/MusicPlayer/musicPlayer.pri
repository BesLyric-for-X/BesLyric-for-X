

#屏蔽 msvc 编译器对 rational.h 的 warning: C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819"


HEADERS+=\
    $$PWD/musicPlayer.h\

SOURCES+=\
    $$PWD/musicPlayer.cpp\



win32{

#根据开发者自己 ffmpeg 和 sdl 库路径，可对如下路径进行修改，不过建议 库安装在 C:/lib 下，
#   具体使用步骤，可参看项目： https://github.com/BensonLaur/beslyric-lib

WIN32_LIB_PATH = C:/lib/beslyric-lib

#ffmpeg

FFMPEG_INCLUDE  =   $$WIN32_LIB_PATH/win32/ffmpeg_4_0_1/include
FFMPEG_LIB      =   $$WIN32_LIB_PATH/win32/ffmpeg_4_0_1/lib
FFMPEG_BIN      =   $$WIN32_LIB_PATH/win32/ffmpeg_4_0_1/bin

#sdl

SDL_INCLUDE     =   $$WIN32_LIB_PATH/SDL_2_0_3/include
SDL_LIB         =   $$WIN32_LIB_PATH/SDL_2_0_3/lib
SDL_BIN         =   $$WIN32_LIB_PATH/SDL_2_0_3/bin

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
        -L$$SDL_LIB/ -lSDL2main  \
        -L$$SDL_LIB/ -lSDL2

# https://stackoverflow.com/questions/6771039/automatic-copy-of-dependent-files-in-qt-creator
# https://doc.qt.io/qt-5/qmake-advanced-usage.html#installing-files

libs_ffmpeg_related.files = $$FFMPEG_BIN/avcodec-58.dll \
                   $$FFMPEG_BIN/avdevice-58.dll \
                   $$FFMPEG_BIN/avfilter-7.dll \
                   $$FFMPEG_BIN/avformat-58.dll \
                   $$FFMPEG_BIN/avutil-56.dll \
                   $$FFMPEG_BIN/postproc-55.dll \
                   $$FFMPEG_BIN/swresample-3.dll \
                   $$FFMPEG_BIN/swscale-5.dll \
                   $$SDL_BIN/SDL2.dll

#.pro 对 CONFIG 做了调整，不再有 debug 和 release 目录分别在 debug 和 release configuration 下生成
CONFIG(debug, debug|release):libs_ffmpeg_related.path = $$OUT_PWD/debug_output
CONFIG(release, debug|release):libs_ffmpeg_related.path = $$OUT_PWD/release_output

#    message(Beslyric-for-X.pro libs_ffmpeg_related.files: $$libs_ffmpeg_related.files)
#    message(Beslyric-for-X.pro libs_ffmpeg_related.path: $$libs_ffmpeg_related.path)

INSTALLS += libs_ffmpeg_related
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
        -L$$SDL_LIB/ -lSDL2main        \   #can't be $$PSDL_LIB/ -lSDL2main, must be -L$$PSDL_LIB/ -lSDL2main
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
        -L$$SDL_LIB/ -lSDL2main        \
        -L$$SDL_LIB/ -lSDL2

}
