
# ------ 3rd party DLLs ------
include(deploy/windows_dll_deploy_3rd-party.pri)

# ------ windeployqt - enhanced ------
include(deploy/windows_dll_deploy_windeployqt-enhanced.pri)

win32-msvc {
    # ------ MSVC CRT ------
    include(deploy/windows_dll_deploy_msvc-crt.pri)
    
    # ------ MSVC UCRT ------
    include(deploy/windows_dll_deploy_msvc-ucrt.pri)
}
