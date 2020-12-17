# ------ 3rd party DLLs ------


include(../define/windows_dll_define_3rd-party.pri)

defineTest(locateThirdPartyDlls) {
    checkArgumentCount("locateThirdPartyDlls", $${ARGC}, 2, 3)

    dlls_dir_path = $${1}
    this_target = $${2}
    target_depends_on_this = $${3}

    for(THIRD_PARTY_DLL, THIRD_PARTY_DLLS) {
        THIRD_PARTY_DLL_SOURCE_PATHS_FORWARDSLASH *= $$getSourcePathForwardSlashed($${dlls_dir_path}, $${THIRD_PARTY_DLL})
        THIRD_PARTY_DLL_SOURCE_PATHS_QUOTED *= $$getSourcePathQuoted($${dlls_dir_path}, $${THIRD_PARTY_DLL})
        THIRD_PARTY_DLL_TARGET_PATHS_QUOTED *= $$getTargetPathQuoted($${THIRD_PARTY_DLL})
    }

    message("THIRD_PARTY_DLL_SOURCE_PATHS_QUOTED = $${THIRD_PARTY_DLL_SOURCE_PATHS_QUOTED}")
    message("THIRD_PARTY_DLL_TARGET_PATHS_QUOTED = $${THIRD_PARTY_DLL_TARGET_PATHS_QUOTED}")

    makeFileDistributionTarget( \
        $${this_target}, \
        $${THIRD_PARTY_DLL_SOURCE_PATHS_QUOTED}, \
        $${THIRD_PARTY_DLL_TARGET_PATHS_QUOTED}, \
        $${target_depends_on_this} \
    )

    message("THIRD_PARTY_DLL_SOURCE_PATHS_FORWARDSLASH = $${THIRD_PARTY_DLL_SOURCE_PATHS_FORWARDSLASH}")

    INSTALLS_$${this_target}.path = "/"
    INSTALLS_$${this_target}.files = $${THIRD_PARTY_DLL_SOURCE_PATHS_FORWARDSLASH}
    INSTALLS_$${this_target}.CONFIG = nostrip

    message("INSTALLS_$${this_target}.path = $$eval(INSTALLS_$${this_target}.path)")
    message("INSTALLS_$${this_target}.files = $$eval(INSTALLS_$${this_target}.files)")
    message("INSTALLS_$${this_target}.CONFIG = $$eval(INSTALLS_$${this_target}.CONFIG)")

    export(INSTALLS_$${this_target}.path)
    export(INSTALLS_$${this_target}.files)
    export(INSTALLS_$${this_target}.CONFIG)

    INSTALLS *= INSTALLS_$${this_target}
    export(INSTALLS)
}
