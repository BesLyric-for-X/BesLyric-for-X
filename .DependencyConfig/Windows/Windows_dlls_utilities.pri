# ------Basic utilities------


# Don't use `DEST_DIR`, it may be undefined.
load(resolve_target)
TARGET_DIR = $$system_path($$dirname(QMAKE_RESOLVED_TARGET))
message("TARGET_DIR = $${TARGET_DIR}")


defineReplace(get_source_path_quoted) {
    !equals(ARGC, 2): \
        error("get_source_path_quoted() requires two arguments")

    base_dir_path = $${1}
    filename = $${2}

    path = $$system_path($$clean_path($$absolute_path($${filename}, $${base_dir_path})))
    !exists($${path}) {
        error("\"$${path}\" is NOT existed.")
    }
    return ($$shell_quote($${path}))
}


defineReplace(get_target_path_quoted) {
    greaterThan(ARGC, 1): \
        error("get_target_path_quoted() requires at most one argument")

    filename = $${1}

    path = $$system_path($$clean_path($$absolute_path($${filename}, $${TARGET_DIR})))
    return ($$shell_quote($${path}))
}


defineTest(make_dll_distribution_target) {
    lessThan(ARGC, 3)|greaterThan(ARGC, 4): \
        error("make_dll_distribution_target() requires three or four arguments")

    target_name = $${1}
    source_files_path_list = $${2}
    target_files_path_list = $${3}
    depends_target = $${4}

    build_pass {
        win32-msvc {
            $${target_name}.commands = \
                for %i in ($${source_files_path_list}) do \
                    $${QMAKE_COPY} %i $$get_target_path_quoted()
            $${target_name}_clean.commands = \
                for %i in ($${target_files_path_list}) do \
                    $${QMAKE_DEL_FILE} %i
        }
        win32-g++ {
            $${target_name}.commands = \
                for i in $${source_files_path_list} ; do \
                    $${QMAKE_COPY} --verbose \$\$i $$get_target_path_quoted() ; \
                done
            $${target_name}_clean.commands = \
                for i in $${target_files_path_list} ; do \
                    $${QMAKE_DEL_FILE} --verbose \$\$i ; \
                done
        }

        DISTCLEAN_DEPS *= $${target_name}_clean

        message("$${target_name}.commands = $$eval($${target_name}.commands)")
        message("$${target_name}_clean.commands = $$eval($${target_name}_clean.commands)")

        # depends_target
        !isEmpty(depends_target) {
            $${depends_target}.depends *= $${target_name}
            $${depends_target}_clean.depends *= $${target_name}_clean
        }
    } else {
        $${target_name}.CONFIG *= recursive
        $${target_name}_clean.CONFIG *= recursive
    }

    make_target_called_only_once_and_depends_on_first($${target_name}, true)
    make_target_called_only_once_and_depends_on_first($${target_name}_clean, false)

    export($${target_name}.commands)
    export($${target_name}_clean.commands)
    export($${target_name}.CONFIG)
    export($${target_name}_clean.CONFIG)
    export($${target_name}.depends)
    export(DISTCLEAN_DEPS)
    export(QMAKE_EXTRA_TARGETS)

    # depends_target
    !isEmpty(depends_target) {
        export($${depends_target}.depends)
        export($${depends_target}_clean.depends)
    }
}


# From: $$[QT_INSTALL_PREFIX]/mkspecs/features/qt_app.prf
#       $$[QT_INSTALL_PREFIX]/mkspecs/features/testcase.prf
#
# Why don't you guys make the documents better?

# You need to check the three files "Makefile", "Makefile.Debug" and "Makefile.Release" to try to understand what the following code does.

# ------Example code------
#QMAKE_EXTRA_TARGETS *= the_target the_target_clean

#isEmpty(BUILDS)|build_pass {
#    the_target.depends = first $${the_target.depends}

#    message(the_target.depends = $${the_target.depends})
#} else {
#    # For exclusive builds, copy dlls only once.
#    the_target.CONFIG = recursive
#    the_target.target = the_target_all
#    the_target.recurse_target = the_target

#     # substitute target
#    the_target_first.depends = $$eval($$first(BUILDS).target)-the_target
#    the_target_first.target = the_target
#    QMAKE_EXTRA_TARGETS *= the_target_first

#    message(the_target_first.depends = $${the_target_first.depends})
#}
# ------Example code------

defineTest(make_target_called_only_once_and_depends_on_first) {
    !equals(ARGC, 2): \
        error("make_target_called_only_once_and_depends_on_first() requires two arguments")

    theTarget = $${1}
    isDependedOnFirst = $${2}

    QMAKE_EXTRA_TARGETS *= $${theTarget}

    isEmpty(BUILDS)|build_pass {
        equals(isDependedOnFirst, true) {
            $${theTarget}.depends = first $$eval($${theTarget}.depends)
        }

        message($${theTarget}.depends = $$eval($${theTarget}.depends))

        export($${theTarget}.depends)
    } else {
        # For exclusive builds, copy dlls only once.
        $${theTarget}.CONFIG = recursive
        $${theTarget}.target = $${theTarget}_all
        $${theTarget}.recurse_target = $${theTarget}

        # substitute target
        $${theTarget}_first.depends = $$eval($$first(BUILDS).target)-$${theTarget}
        $${theTarget}_first.target = $${theTarget}
        QMAKE_EXTRA_TARGETS *= $${theTarget}_first

        message($${theTarget}_first.depends = $$eval($${theTarget}_first.depends))

        export($${theTarget}.CONFIG)
        export($${theTarget}.target)
        export($${theTarget}.recurse_target)
        export($${theTarget}_first.depends)
        export($${theTarget}_first.target)
    }

    export(QMAKE_EXTRA_TARGETS)
}
