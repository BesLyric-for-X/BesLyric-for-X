# VS 2017  MSVC++ 14.1  _MSC_VER == 1910 (Visual Studio 2017 version 15.0) and higher
# Windows SDK 10.0.17763.0 and higher
# Mingw-w64-{ i686 | x86_64 }

# Learnt from:
#   https://doc.qt.io/qt-5/qmake-advanced-usage.html#adding-custom-targets
#   $$[QT_INSTALL_PREFIX]/mkspecs/features/win32/windeployqt.prf
#   $$[QT_INSTALL_PREFIX]/mkspecs/features/qt_functions.prf
#   $$[QT_INSTALL_PREFIX]/mkspecs/features/qt_app.prf
#   $$[QT_INSTALL_PREFIX]/mkspecs/features/testcase.prf
#   https://doc.qt.io/qt-5/qmake-environment-reference.html


# Attention!
#   Mingw-w64 is not fully supported. CQtDeployer may be used by us in the future.


# Environment variable `B4X_DEP_PATH` is needed.
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
message(B4X_DEP_PATH = $${B4X_DEP_PATH})
!exists($${B4X_DEP_PATH}) {
    error("\"$${B4X_DEP_PATH}\" is NOT existed.")
}


# ------Headers and Libs------

include(Windows_includes_libs.pri)

win32-msvc {
    locate_headers_and_libs($${B4X_DEP_PATH})
}
win32-g++ {
    locate_headers_and_libs()
}

# ------DLLs------

B4X_DEP_PATH_BIN = $$system_path($${B4X_DEP_PATH}/bin) # For Mingw-w64, `$$[QT_INSTALL_PREFIX]/bin` (or `$$[QT_INSTALL_BINS]`) is our home.
message(B4X_DEP_PATH_BIN = $${B4X_DEP_PATH_BIN})
!exists($${B4X_DEP_PATH_BIN}) {
    error("\"$${B4X_DEP_PATH_BIN}\" is NOT existed.")
}

# ------Basic utilities------
include(Windows_dlls_utilities.pri)

# ------Targets------
MAIN_TARGET = deploy_windows_dlls

THIRD_PARTY_LIBRARY_DEPLOYMENT_TARGET = deploy_3rd_party_library_dlls
WINDEPLOYQT_ENHANCED_TARGET = windeployqt_enhanced
CRT_DEPLOYMENT_TARGET = deploy_CRT_dlls
UCRT_DEPLOYMENT_TARGET = deploy_UCRT_dlls

make_target_called_only_once_and_depends_on_first( \
    $${MAIN_TARGET}, \
    true \
)
make_target_called_only_once_and_depends_on_first( \
    $${MAIN_TARGET}_clean, \
    false \
)

# ------3rd party DLL------
include(Windows_dlls_deploys/Windows_dlls_deploy_3rd-party.pri)

locate_third_party_dlls( \
    $${B4X_DEP_PATH_BIN}, \
    $${THIRD_PARTY_LIBRARY_DEPLOYMENT_TARGET}, \
    $${MAIN_TARGET} \
)

# ------windeployqt - enhanced------
include(Windows_dlls_deploys/Windows_dlls_deploy_windeployqt-enhanced.pri)

invoke_windeployqt_enhanced( \
    $${WINDEPLOYQT_ENHANCED_TARGET}, \
    $${MAIN_TARGET} \
)

win32-msvc {
    # ------MSVC CRT------
    include(Windows_dlls_deploys/Windows_dlls_deploy_msvc-crt.pri)

    locate_msvc_crt_dlls( \
        $$getenv(VCToolsVersion), \
        $${CRT_DEPLOYMENT_TARGET}, \
        $${MAIN_TARGET} \
    )

    # ------MSVC UCRT------
    include(Windows_dlls_deploys/Windows_dlls_deploy_msvc-ucrt.pri)

    locate_msvc_ucrt_dlls( \
        $$getenv(UCRTVersion), \
        $${UCRT_DEPLOYMENT_TARGET}, \
        $${MAIN_TARGET} \
    )
}
