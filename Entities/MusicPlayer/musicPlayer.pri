
HEADERS+=\
    $$PWD/musicPlayer.h\
   


SOURCES+=\
    $$PWD/musicPlayer.cpp\


win32{

#根据开发者自己 ffmpeg 和 sdl 库路径，可对如下路径进行修改，不过建议 库安装在 C:/lib 下，
#   具体使用步骤，可参看项目： https://github.com/BensonLaur/beslyic-lib

#ffmpeg

FFMPEG_INCLUDE  =   C:/lib/beslyic-lib/win32/ffmpeg_4_0_1/include
FFMPEG_LIB      =   C:/lib/beslyic-lib/win32/ffmpeg_4_0_1/lib

#sdl

SDL_INCLUDE     =   C:/lib/beslyic-lib/win32/SDL_2_0_3/include
SDL_LIB         =   C:/lib/beslyic-lib/win32/SDL_2_0_3/lib

#other
OTHER_INCLUDE   =   C:/lib/beslyic-lib/win32/include

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
        -L$$FFMPEG_LIB/ -swresample \
        -L$$SDL_LIB/ -lSDL2main  \
        -L$$SDL_LIB/ -lSDL2
}
