# ------CRT------


include(../Windows_dlls_defines/Windows_dlls_defines_msvc-crt.pri)

defineTest(locate_msvc_crt_dlls) {
    lessThan(ARGC, 2)|greaterThan(ARGC, 3): \
        error("locate_msvc_crt_dlls() requires two or three arguments")
        
    VCToolsVersion = $${1}
    this_target = $${2}
    depends_target = $${3}

    isEmpty(VCToolsVersion) {
        error("Unknown env VCToolsVersion")
    }
    !versionAtLeast(VCToolsVersion, 14.1) {
        error("Your MSVC version ($$VCToolsVersion) is too old, you should install Visual Studio 2017 or higher version.")
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

    isEqual(VC_THREE_NUMBER_VERSION, 142) {
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
        CRT_DLL_SOURCE_PATHS_QUOTED *= $$get_source_path_quoted($${CRT_DIR}, $${CRT_DLL})
        CRT_DLL_TARGET_PATHS_QUOTED *= $$get_target_path_quoted($${CRT_DLL})
    }

    message("CRT_DLL_SOURCE_PATHS_QUOTED = $${CRT_DLL_SOURCE_PATHS_QUOTED}")
    message("CRT_DLL_TARGET_PATHS_QUOTED = $${CRT_DLL_TARGET_PATHS_QUOTED}")

    make_dll_distribution_target( \
        $${this_target}, \
        $${CRT_DLL_SOURCE_PATHS_QUOTED}, \
        $${CRT_DLL_TARGET_PATHS_QUOTED}, \
        $${depends_target} \
    )
}
