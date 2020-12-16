# ------ Headers and Libs ------


defineTest(locateHeadersAndLibs) {
    win32-msvc {
        checkArgumentCount("locateHeadersAndLibs", $${ARGC}, 1)

        base_path = $${1}

        include_path = $$system_path($${base_path}/include)
        lib_path = $$system_path($${base_path}/lib)

        !exists($${include_path}) {
            error("\"$${include_path}\" is NOT existed.")
        }

        !exists($${lib_path}) {
            error("\"$${lib_path}\" is NOT existed.")
        }

        INCLUDEPATH *= $${include_path}
        LIBS *= \
            -L$${lib_path} \
            -lavcodec \
            -lavdevice \
            -lavfilter \
            -lavformat \
            -lavutil \
            -lpostproc \
            -lswresample \
            -lswscale \
            -lSDL2 \
            -lSDL2main

        message("INCLUDEPATH = $${INCLUDEPATH}")
        message("LIBS = $${LIBS}")

        export(INCLUDEPATH)
        export(LIBS)
    }
    win32-g++ {
        checkArgumentCount("locateHeadersAndLibs", $${ARGC}, 0)

        # pkg-config is needed.

        # include(..\Linux\Linux_common.pri)

        CONFIG *= link_pkgconfig
        PKGCONFIG *= \
            libavcodec \
            libavdevice \
            libavfilter \
            libavformat \
            libavutil \
            libpostproc \
            libswresample \
            libswscale \
            sdl2

        export(CONFIG)
        export(PKGCONFIG)
    }
}
