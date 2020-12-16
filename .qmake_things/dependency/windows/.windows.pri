# VS 2017 (MSVC++ 14.1, _MSC_VER == 1910, or Visual Studio 2017 version 15.0) and later.
# Windows SDK 10.0.17763.0 and later.

# Mingw-w64-{ i686 | x86_64 }.

# Learnt from:
#   https://doc.qt.io/qt-5/qmake-advanced-usage.html#adding-custom-targets
#   $$[QT_INSTALL_PREFIX]/mkspecs/features/win32/windeployqt.prf
#   $$[QT_INSTALL_PREFIX]/mkspecs/features/qt_functions.prf
#   $$[QT_INSTALL_PREFIX]/mkspecs/features/qt_app.prf
#   $$[QT_INSTALL_PREFIX]/mkspecs/features/testcase.prf
#   https://doc.qt.io/qt-5/qmake-environment-reference.html


# Attention!
#   Mingw-w64 is not fully supported by us. CQtDeployer may be used as a helper in the future.


# The environment variable `B4X_DEP_PATH` is required by Windows MSVC while it is optional for Windows Mingw-w64.
#   For `B4X_DEP_PATH`: ASCII, no accents, spaces nor symlinks, short path.


B4X_DEP_PATH = $$getenv(B4X_DEP_PATH)

isEmpty(B4X_DEP_PATH) {
    win32-msvc {
        error("env \"B4X_DEP_PATH\" is NOT set.")
    }
    win32-g++ {
        B4X_DEP_PATH = $$system_path($$[QT_INSTALL_PREFIX]) # I give you a default value.
        message("env \"B4X_DEP_PATH\" is NOT set, try \"$${B4X_DEP_PATH}\".")
    }
}

contains(B4X_DEP_PATH, "^\s*$") {
    error("\"B4X_DEP_PATH\" is empty.")
}

B4X_DEP_PATH = $$system_path($$absolute_path($$clean_path($${B4X_DEP_PATH})))
message("B4X_DEP_PATH = $${B4X_DEP_PATH}")
!exists($${B4X_DEP_PATH}) {
    error("\"$${B4X_DEP_PATH}\" is NOT existed.")
}


# ------ Headers and Libs ------

include(header_and_lib/windows_header_and_lib.pri)

win32-msvc {
    locateHeadersAndLibs($${B4X_DEP_PATH})
}
win32-g++ {
    locateHeadersAndLibs()
}

# ------ DLLs ------

include(dll/windows_dll.pri)

B4X_DEP_PATH_BIN = $$system_path($${B4X_DEP_PATH}/bin) # For Mingw-w64, `$$[QT_INSTALL_PREFIX]/bin` (or `$$[QT_INSTALL_BINS]`) is our home.
message("B4X_DEP_PATH_BIN = $${B4X_DEP_PATH_BIN}")
!exists($${B4X_DEP_PATH_BIN}) {
    error("\"$${B4X_DEP_PATH_BIN}\" is NOT existed.")
}

# ------ Custom targets ------
MAIN_TARGET = deploy_windows_dlls

THIRD_PARTY_LIBRARY_DEPLOYMENT_TARGET = deploy_3rd_party_library_dlls
WINDEPLOYQT_ENHANCED_TARGET = windeployqt_enhanced
CRT_DEPLOYMENT_TARGET = deploy_CRT_dlls
UCRT_DEPLOYMENT_TARGET = deploy_UCRT_dlls

makeTargetCalledOnlyOnceAndDependsOnFirst($${MAIN_TARGET}, true)
makeTargetCalledOnlyOnceAndDependsOnFirst($${MAIN_TARGET}_clean, false)

# ------ 3rd party DLLs ------
locateThirdPartyDlls( \
    $${B4X_DEP_PATH_BIN}, \
    $${THIRD_PARTY_LIBRARY_DEPLOYMENT_TARGET}, \
    $${MAIN_TARGET} \
)

# ------ windeployqt - enhanced ------
invokeWindeployqtEnhanced( \
    $${WINDEPLOYQT_ENHANCED_TARGET}, \
    $${MAIN_TARGET} \
)

win32-msvc {
    # ------ MSVC CRT ------
    locateMsvcCrtDlls( \
        $$getenv(VCToolsVersion), \
        $${CRT_DEPLOYMENT_TARGET}, \
        $${MAIN_TARGET} \
    )

    # ------ MSVC UCRT ------
    locateMsvcUcrtDlls( \
        $$getenv(UCRTVersion), \
        $${UCRT_DEPLOYMENT_TARGET}, \
        $${MAIN_TARGET} \
    )
}
