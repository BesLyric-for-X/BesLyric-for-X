# ------ MSVC CRT ------


include(../define/windows_dll_define_msvc-crt.pri)

defineTest(locateMsvcCrtDlls) {
    checkArgumentCount("locateMsvcCrtDlls", $${ARGC}, 2, 3)

    VCToolsVersion = $${1}
    this_target = $${2}
    target_depends_on_this = $${3}

    isEmpty(VCToolsVersion) {
        error("Unknown env VCToolsVersion")
    }
    !versionAtLeast(VCToolsVersion, 14.1) {
        error("Your MSVC version ($${VCToolsVersion}) is too old, you should install Visual Studio 2017 or later version.")
    }
    lessThan(VCToolsVersion, 14.2) {
        VC_THREE_NUMBER_VERSION = 141 # MSVC v141 (14.1*)
    } else:lessThan(VCToolsVersion, 14.3) {
        VC_THREE_NUMBER_VERSION = 142 # MSVC v142 (14.2*)
    } else {
        error("Unknown MSVC version.")
    }

    message("VCToolsVersion = $${VCToolsVersion}")

    CRT_DEBUG_DIR = $$system_path($$clean_path($$getenv(VCToolsRedistDir)/debug_nonredist/$$getenv(Platform)/Microsoft.VC$${VC_THREE_NUMBER_VERSION}.DebugCRT))
    CRT_RELEASE_DIR = $$system_path($$clean_path($$getenv(VCToolsRedistDir)/$$getenv(Platform)/Microsoft.VC$${VC_THREE_NUMBER_VERSION}.CRT))

    !exists($${CRT_DEBUG_DIR}) {
        error("\"$${CRT_DEBUG_DIR}\" is NOT existed.")
    }
    !exists($${CRT_RELEASE_DIR}) {
        error("\"$${CRT_RELEASE_DIR}\" is NOT existed.")
    }

    message("CRT_DEBUG_DIR = $${CRT_DEBUG_DIR}")
    message("CRT_RELEASE_DIR = $${CRT_RELEASE_DIR}")

    equals(VC_THREE_NUMBER_VERSION, 142) {
        CRT_DEBUG_DLLS *= $${CRT_v142_DEBUG_DLLS}
        CRT_RELEASE_DLLS *= $${CRT_v142_RELEASE_DLLS}
    }

    CONFIG(debug, debug|release) {
        CRT_DIR = $${CRT_DEBUG_DIR}
        CRT_DLLS = $${CRT_DEBUG_DLLS}
    } else:CONFIG(release, debug|release) {
        CRT_DIR = $${CRT_RELEASE_DIR}
        CRT_DLLS = $${CRT_RELEASE_DLLS}
    }

    for(CRT_DLL, CRT_DLLS) {
        CRT_DLL_SOURCE_PATHS_QUOTED *= $$getSourcePathQuoted($${CRT_DIR}, $${CRT_DLL})
        CRT_DLL_TARGET_PATHS_QUOTED *= $$getTargetPathQuoted($${CRT_DLL})
    }

    message("CRT_DLL_SOURCE_PATHS_QUOTED = $${CRT_DLL_SOURCE_PATHS_QUOTED}")
    message("CRT_DLL_TARGET_PATHS_QUOTED = $${CRT_DLL_TARGET_PATHS_QUOTED}")

    makeFileDistributionTarget( \
        $${this_target}, \
        $${CRT_DLL_SOURCE_PATHS_QUOTED}, \
        $${CRT_DLL_TARGET_PATHS_QUOTED}, \
        $${target_depends_on_this} \
    )
}
