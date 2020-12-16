# ------ MSVC UCRT ------


include(../define/windows_dll_define_msvc-ucrt.pri)

defineTest(locateMsvcUcrtDlls) {
    checkArgumentCount("locateMsvcUcrtDlls", $${ARGC}, 2, 3)

    UCRTVersion = $${1}
    this_target = $${2}
    target_depends_on_this = $${3}

    isEmpty(UCRTVersion) {
        error("Unknown env UCRTVersion")
    }
    !versionAtLeast(UCRTVersion, 10.0.17763.0) {
        error("Your UCRT version ($${UCRTVersion}) is too old, you should install Windows SDK 10.0.17763.0 or later version.")
    }

    message("UCRTVersion = $${UCRTVersion}")

    UCRT_DEBUG_DIR = $$system_path($$clean_path($$getenv(WindowsSdkDir)/bin/$${UCRTVersion}/$$getenv(Platform)/ucrt))
    UCRT_RELEASE_DIR = $$system_path($$clean_path($$getenv(WindowsSdkDir)/Redist/$${UCRTVersion}/ucrt/DLLs/$$getenv(Platform)))
    UCRT_COMMON_DIR = $${UCRT_RELEASE_DIR}

    !exists($${UCRT_DEBUG_DIR}) {
        error("\"$${UCRT_DEBUG_DIR}\" is NOT existed.")
    }
    !exists($${UCRT_RELEASE_DIR}) {
        error("\"$${UCRT_RELEASE_DIR}\" is NOT existed.")
    }

    message("UCRT_DEBUG_DIR = $${UCRT_DEBUG_DIR}")
    message("UCRT_RELEASE_DIR = $${UCRT_RELEASE_DIR}")

    for(UCRT_COMMON_DLL, UCRT_COMMON_DLLS) {
        UCRT_DLL_SOURCE_PATHS_QUOTED *= $$getSourcePathQuoted($${UCRT_COMMON_DIR}, $${UCRT_COMMON_DLL})
        UCRT_DLL_TARGET_PATHS_QUOTED *= $$getTargetPathQuoted($${UCRT_COMMON_DLL})
    }

    # For now, only ucrtbase(d).dll in UCRT_{ DEBUG | RELEASE }_DLLS.
    CONFIG(debug, debug|release) {
        UCRT_DIR = $${UCRT_DEBUG_DIR}
        UCRT_DLLS = $${UCRT_DEBUG_DLLS}
    } else:CONFIG(release, debug|release) {
        UCRT_DIR = $${UCRT_RELEASE_DIR}
        UCRT_DLLS = $${UCRT_RELEASE_DLLS}
    }

    for(UCRT_DLL, UCRT_DLLS) {
        UCRT_DLL_SOURCE_PATHS_QUOTED *= $$getSourcePathQuoted($${UCRT_DIR}, $${UCRT_DLL})
        UCRT_DLL_TARGET_PATHS_QUOTED *= $$getTargetPathQuoted($${UCRT_DLL})
    }

    message("UCRT_DLL_SOURCE_PATHS_QUOTED = $${UCRT_DLL_SOURCE_PATHS_QUOTED}")
    message("UCRT_DLL_TARGET_PATHS_QUOTED = $${UCRT_DLL_TARGET_PATHS_QUOTED}")

    makeFileDistributionTarget( \
        $${this_target}, \
        $${UCRT_DLL_SOURCE_PATHS_QUOTED}, \
        $${UCRT_DLL_TARGET_PATHS_QUOTED}, \
        $${target_depends_on_this} \
    )
}
