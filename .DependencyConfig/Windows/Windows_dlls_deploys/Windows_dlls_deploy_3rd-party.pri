# ------3rd party DLL------


include(../Windows_dlls_defines/Windows_dlls_defines_3rd-party.pri)

defineTest(locate_third_party_dlls) {
    lessThan(ARGC, 2)|greaterThan(ARGC, 3): \
        error("locate_third_party_dlls() requires two or three arguments")

    dlls_dir_path = $${1}
    this_target = $${2}
    depends_target = $${3}

    for(THIRD_PARTY_DLL, THIRD_PARTY_DLLS) {
        THIRD_PARTY_DLL_SOURCE_PATHS_QUOTED *= $$get_source_path_quoted($${dlls_dir_path}, $${THIRD_PARTY_DLL})
        THIRD_PARTY_DLL_TARGET_PATHS_QUOTED *= $$get_target_path_quoted($${THIRD_PARTY_DLL})
    }

    message("THIRD_PARTY_DLL_SOURCE_PATHS_QUOTED = $${THIRD_PARTY_DLL_SOURCE_PATHS_QUOTED}")
    message("THIRD_PARTY_DLL_TARGET_PATHS_QUOTED = $${THIRD_PARTY_DLL_TARGET_PATHS_QUOTED}")

    make_dll_distribution_target( \
        $${this_target}, \
        $${THIRD_PARTY_DLL_SOURCE_PATHS_QUOTED}, \
        $${THIRD_PARTY_DLL_TARGET_PATHS_QUOTED}, \
        $${depends_target} \
    )
}
