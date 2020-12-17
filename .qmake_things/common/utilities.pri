# Common Utilities


defineTest(checkArgumentCount) {
    lessThan(ARGC, 3)|greaterThan(ARGC, 4) {
        checkArgumentCount('checkArgumentCount', $${ARGC}, 3, 4)
    }

    function_name = $${1}
    argc_actual = $${2}
    argc_expected_minimum = $${3}
    argc_expected_maximum = $${4}

    isEmpty(function_name)|contains(function_name, "^\s*$") {
        error("Function \"checkArgumentCount\": the 1st parameter \"function_name\" is empty.")
    }
    isEmpty(argc_actual)|contains(argc_actual, "^\s*$") {
        error("Function \"checkArgumentCount\": the 2rd parameter \"argc_actual\" is empty.")
    }
    isEmpty(argc_expected_minimum)|contains(argc_expected_minimum, "^\s*$") {
        error("Function \"checkArgumentCount\": the 3rd parameter \"argc_expected_minimum\" is empty.")
    }

    isEmpty(argc_expected_maximum) {
        argc_expected_maximum = $${argc_expected_minimum}
    } else {
        contains(argc_expected_maximum, "^\s*$") {
            error("Function \"checkArgumentCount\": the 4th parameter \"argc_expected_maximum\" is empty.")
        }
    }

    lessThan(argc_expected_maximum, $$eval(argc_expected_minimum)) {
        temp_variable = $${argc_expected_maximum}
        argc_expected_maximum = $${argc_expected_minimum}
        argc_expected_minimum = $${temp_variable}
        unset(temp_variable)
    }

    equals(argc_expected_minimum, $$eval(argc_expected_maximum)) {
        expected_argc_message = "$${argc_expected_maximum} parameter"
    } else {
        expected_argc_message = "$${argc_expected_minimum} to $${argc_expected_maximum} parameter"
    }
    greaterThan(argc_expected_maximum, 1) {
        expected_argc_message = "$${expected_argc_message}s"
    }

    lessThan(argc_actual, $$eval(argc_expected_minimum))|greaterThan(argc_actual, $$eval(argc_expected_maximum)) {
        error("Function \"$${function_name}\": Expected $${expected_argc_message}, but got $${argc_actual}.")
    }
}


# Don't use `DEST_DIR`, it may be undefined.
load(resolve_target)
TARGET_DIR = $$system_path($$dirname(QMAKE_RESOLVED_TARGET))
message("TARGET_DIR = $${TARGET_DIR}")


defineReplace(getSourcePathForwardSlashed) {
    checkArgumentCount("getSourcePathForwardSlashed", $${ARGC}, 2)

    base_dir_path = $${1}
    filename = $${2}

    path = $$clean_path($$absolute_path($${filename}, $${base_dir_path}))
    !exists($${path}) {
        error("\"$${path}\" is NOT existed.")
    }
    return ($${path})
}


defineReplace(getSourcePathQuoted) {
    checkArgumentCount("getSourcePathQuoted", $${ARGC}, 2)

    base_dir_path = $${1}
    filename = $${2}

    path = $$system_path($$clean_path($$absolute_path($${filename}, $${base_dir_path})))
    !exists($${path}) {
        error("\"$${path}\" is NOT existed.")
    }
    return ($$shell_quote($${path}))
}


defineReplace(getTargetPathQuoted) {
    checkArgumentCount("getTargetPathQuoted", $${ARGC}, 0, 1)

    filename = $${1}

    path = $$system_path($$clean_path($$absolute_path($${filename}, $${TARGET_DIR})))
    return ($$shell_quote($${path}))
}


defineReplace(getOutputPathQuoted) {
    checkArgumentCount("getOutputPathQuoted", $${ARGC}, 0, 1)

    filename = $${1}

    path = $$system_path($$clean_path($$absolute_path($${filename}, $${OUT_PWD})))
    return ($$shell_quote($${path}))
}


# From: $$[QT_INSTALL_PREFIX]/mkspecs/features/qt_app.prf
#       $$[QT_INSTALL_PREFIX]/mkspecs/features/testcase.prf
#
# Why don't you guys make the documents better?

# You need to check the three files "Makefile", "Makefile.Debug" and "Makefile.Release" to try to understand what the following code did.

# ------ Example code ------
# QMAKE_EXTRA_TARGETS *= this_target this_target_clean

# isEmpty(BUILDS)|build_pass {
#     this_target.depends = first $${this_target.depends}

#     message("this_target.depends = $${this_target.depends}")
# } else {
#     # For exclusive builds, copy dlls only once.
#     this_target.CONFIG = recursive
#     this_target.target = this_target_all
#     this_target.recurse_target = this_target

#     # Substitute targets.
#     this_target_first.depends = $$eval($$first(BUILDS).target)-this_target
#     this_target_first.target = this_target
#     QMAKE_EXTRA_TARGETS *= this_target_first

#     message("this_target_first.depends = $${this_target_first.depends}")
# }
# ------ Example code ------

defineTest(makeTargetCalledOnlyOnceAndDependsOnFirst) {
    checkArgumentCount("makeTargetCalledOnlyOnceAndDependsOnFirst", $${ARGC}, 2)

    this_target = $${1}
    is_dependent_on_first = $${2}

    QMAKE_EXTRA_TARGETS *= $${this_target}

    isEmpty(BUILDS)|build_pass {
        equals(is_dependent_on_first, true) {
            $${this_target}.depends = first $$eval($${this_target}.depends)
        }

        message("$${this_target}.depends = $$eval($${this_target}.depends)")

        export($${this_target}.depends)
    } else {
        # For exclusive builds, copy dlls only once.
        $${this_target}.CONFIG = recursive
        $${this_target}.target = $${this_target}_all
        $${this_target}.recurse_target = $${this_target}

        # Substitute targets.
        $${this_target}_first.depends = $$eval($$first(BUILDS).target)-$${this_target}
        $${this_target}_first.target = $${this_target}
        QMAKE_EXTRA_TARGETS *= $${this_target}_first

        message("$${this_target}_first.depends = $$eval($${this_target}_first.depends)")

        export($${this_target}.CONFIG)
        export($${this_target}.target)
        export($${this_target}.recurse_target)
        export($${this_target}_first.depends)
        export($${this_target}_first.target)
    }

    export(QMAKE_EXTRA_TARGETS)
}


defineTest(makeFileDistributionTarget) {
    !win32 {
        error("Function \"makeFileDistributionTarget\" currently only supports \"win32\" platform.")
    }

    checkArgumentCount("makeFileDistributionTarget", $${ARGC}, 3, 4)

    this_target = $${1}
    source_files_path_list = $${2}
    target_files_path_list = $${3}
    target_depends_on_this = $${4}

    build_pass {
        win32-msvc {
            $${this_target}.commands = \
                for %i in ($${source_files_path_list}) do \
                    $${QMAKE_COPY} %i $$getTargetPathQuoted()
            $${this_target}_clean.commands = \
                for %i in ($${target_files_path_list}) do \
                    $${QMAKE_DEL_FILE} %i
        }
        win32-g++ {
            $${this_target}.commands = \
                for i in $${source_files_path_list} ; do \
                    $${QMAKE_COPY} --verbose \$\$i $$getTargetPathQuoted() ; \
                done
            $${this_target}_clean.commands = \
                for i in $${target_files_path_list} ; do \
                    $${QMAKE_DEL_FILE} --verbose \$\$i ; \
                done
        }

        DISTCLEAN_DEPS *= $${this_target}_clean

        message("$${this_target}.commands = $$eval($${this_target}.commands)")
        message("$${this_target}_clean.commands = $$eval($${this_target}_clean.commands)")

        !isEmpty(target_depends_on_this) {
            $${target_depends_on_this}.depends *= $${this_target}
            $${target_depends_on_this}_clean.depends *= $${this_target}_clean
        }
    } else {
        $${this_target}.CONFIG *= recursive
        $${this_target}_clean.CONFIG *= recursive
    }

    makeTargetCalledOnlyOnceAndDependsOnFirst($${this_target}, false)
    makeTargetCalledOnlyOnceAndDependsOnFirst($${this_target}_clean, false)

    export($${this_target}.commands)
    export($${this_target}_clean.commands)
    export($${this_target}.CONFIG)
    export($${this_target}_clean.CONFIG)
    export($${this_target}.depends)
    export(DISTCLEAN_DEPS)
    export(QMAKE_EXTRA_TARGETS)

    !isEmpty(target_depends_on_this) {
        export($${target_depends_on_this}.depends)
        export($${target_depends_on_this}_clean.depends)
    }
}
