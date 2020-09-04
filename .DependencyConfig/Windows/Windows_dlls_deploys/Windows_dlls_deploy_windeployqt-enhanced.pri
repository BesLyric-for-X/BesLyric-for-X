# ------windeployqt - enhanced------


defineTest(invoke_windeployqt_enhanced) {
    lessThan(ARGC, 1)|greaterThan(ARGC, 2): \
        error("invoke_windeployqt_enhanced() requires one or two arguments")

    this_target = $${1}
    depends_target = $${2}
        
    windeployqt {
        error("\"windeployqt\" in CONFIG is not allowed.")
    }
    # From: https://bugreports.qt.io/browse/QTBUG-81982
    qtPrepareTool(QMAKE_WINDEPLOYQT, windeployqt)
    build_pass {
        isEmpty(WINDEPLOYQT_OPTIONS) {
            win32-msvc{
                # I don't like vcredist_x**.exe
                WINDEPLOYQT_OPTIONS = --qmldir $$shell_quote($$system_path($${_PRO_FILE_PWD_})) --no-compiler-runtime
            }
            win32-g++ {
                # We do need libgcc_s_seh-1.dll, libstdc++-6.dll and libwinpthread-1.dll
                WINDEPLOYQT_OPTIONS = --qmldir $$shell_quote($$system_path($${_PRO_FILE_PWD_})) --compiler-runtime
            }
        }
        WINDEPLOYQT_TARGET = $$get_target_path_quoted()
        WINDEPLOYQT_OUTPUT = $$get_target_path_quoted($$basename(TARGET).windeployqt_enhanced)
        $${this_target}.commands = $${QMAKE_WINDEPLOYQT} $${WINDEPLOYQT_OPTIONS} --list target $${WINDEPLOYQT_TARGET} > $${WINDEPLOYQT_OUTPUT}

        win32-msvc {
            $${this_target}_clean.commands = if exist $${WINDEPLOYQT_OUTPUT} \
                                                for /f \"usebackq delims=\" %i in ($${WINDEPLOYQT_OUTPUT}) do \
                                                    $${QMAKE_DEL_FILE} \"%~fi\" && $${QMAKE_DEL_DIR} \"%~dpi\" || ver > $${QMAKE_SHELL_NULL_DEVICE}
        }
        win32-g++ {
            $${this_target}_clean.commands = test -e $$WINDEPLOYQT_OUTPUT && \
                                                while IFS=\$\$\'\r\n\' read -r filepath; do \
                                                    $$QMAKE_DEL_FILE --verbose \"\$\$filepath\"; $$QMAKE_DEL_FILE --dir --verbose \$\$(dirname \"\$\$filepath\"); \
                                                done < $$WINDEPLOYQT_OUTPUT

            # Don't use `rmdir` ($$QMAKE_DEL_DIR). If the directory does not existing, `rmdir` will return non-zero value and let 'make' fail.
            #
            # '\$\$' is used to generate a single '$' in 'make' process (It's a `sh -c` command).
            #   The process: \$\$ (.prf) -(`qmake`)-> $$ (Makefile) -(`sh -c`)-> $ (shell)
        }

        DISTCLEAN_DEPS *= $${this_target}_clean
        QMAKE_DISTCLEAN *= $${WINDEPLOYQT_OUTPUT}

        message("WINDEPLOYQT_OPTIONS = $${WINDEPLOYQT_OPTIONS}")
        message("WINDEPLOYQT_OUTPUT = $${WINDEPLOYQT_OUTPUT}")
        message("$${this_target}.commands = $$eval($${this_target}.commands)")
        message("$${this_target}_clean.commands = $$eval($${this_target}_clean.commands)")

        !isEmpty(depends_target) {
            $${depends_target}.depends *= $${this_target}
            $${depends_target}_clean.depends *= $${this_target}_clean
        }
    } else {
        $${this_target}.CONFIG *= recursive
        $${this_target}_clean.CONFIG *= recursive
    }

    make_target_called_only_once_and_depends_on_first($${this_target}, true)
    make_target_called_only_once_and_depends_on_first($${this_target}_clean, false)

    export($${this_target}.commands)
    export($${this_target}_clean.commands)
    export($${this_target}.CONFIG)
    export($${this_target}_clean.CONFIG)
    export($${this_target}.depends)
    export(DISTCLEAN_DEPS)
    export(QMAKE_DISTCLEAN)
    export(QMAKE_EXTRA_TARGETS)

    !isEmpty(depends_target) {
        export($${depends_target}.depends)
        export($${depends_target}_clean.depends)
    }
}
