# ------UCRT------


include(../Windows_dlls_defines/Windows_dlls_defines_msvc-ucrt.pri)

defineTest(locate_msvc_ucrt_dlls) {
    lessThan(ARGC, 2)|greaterThan(ARGC, 3): \
        error("locate_msvc_ucrt_dlls() requires two or three arguments")

    UCRTVersion = $${1}
    this_target = $${2}
    depends_target = $${3}

    isEmpty(UCRTVersion) {
        error("Unknown env UCRTVersion")
    }
    !versionAtLeast(UCRTVersion, 10.0.17763.0) {
        error("Your UCRT version ($$UCRTVersion) is too old, you should install Windows SDK 10.0.17763.0 or higher version.")
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
        UCRT_DLL_SOURCE_PATHS_QUOTED *= $$get_source_path_quoted($${UCRT_COMMON_DIR}, $${UCRT_COMMON_DLL})
        UCRT_DLL_TARGET_PATHS_QUOTED *= $$get_target_path_quoted($${UCRT_COMMON_DLL})
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
        UCRT_DLL_SOURCE_PATHS_QUOTED *= $$get_source_path_quoted($${UCRT_DIR}, $${UCRT_DLL})
        UCRT_DLL_TARGET_PATHS_QUOTED *= $$get_target_path_quoted($${UCRT_DLL})
    }

    message("UCRT_DLL_SOURCE_PATHS_QUOTED = $${UCRT_DLL_SOURCE_PATHS_QUOTED}")
    message("UCRT_DLL_TARGET_PATHS_QUOTED = $${UCRT_DLL_TARGET_PATHS_QUOTED}")

    make_dll_distribution_target( \
        $${this_target}, \
        $${UCRT_DLL_SOURCE_PATHS_QUOTED}, \
        $${UCRT_DLL_TARGET_PATHS_QUOTED}, \
        $${depends_target} \
    )
}
