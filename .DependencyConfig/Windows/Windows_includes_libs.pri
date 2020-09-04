# ------Headers and Libs------


defineTest(locate_headers_and_libs) {
    win32-msvc {
        !equals(ARGC, 1): \
            error("locate_headers_and_libs() requires one argument")

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
        LIBS *= -L$${lib_path} \
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

        message("INCLUDEPATH = $$INCLUDEPATH")
        message("LIBS = $$LIBS")

        export(INCLUDEPATH)
        export(LIBS)
    }
    win32-g++ {
        !equals(ARGC, 0): \
            error("locate_headers_and_libs() requires no argument")

        # pkg-config is needed.

        # include(..\Linux\Linux_common.pri)

        CONFIG *= link_pkgconfig
        PKGCONFIG *= libavcodec \
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
