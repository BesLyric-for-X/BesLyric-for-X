# ------ MSVC CRT ------


CRT_DEBUG_DLLS = \
    concrt140d.dll \
    msvcp140_1d.dll \
    msvcp140_2d.dll \
    msvcp140d.dll \
    vccorlib140d.dll \
    vcruntime140d.dll

CRT_RELEASE_DLLS = \
    concrt140.dll \
    msvcp140_1.dll \
    msvcp140_2.dll \
    msvcp140.dll \
    vccorlib140.dll \
    vcruntime140.dll

CRT_v142_DEBUG_DLLS = \
    msvcp140d_codecvt_ids.dll

CRT_v142_RELEASE_DLLS = \
    msvcp140_codecvt_ids.dll

CRT_v142_X64_DEBUG_DLLS = \
    vcruntime140_1d.dll

CRT_v142_X64_RELEASE_DLLS = \
    vcruntime140_1.dll

equals(QT_ARCH, i386) {}
else:equals(QT_ARCH, x86_64) {
    CRT_v142_DEBUG_DLLS *= $${CRT_v142_X64_DEBUG_DLLS}
    CRT_v142_RELEASE_DLLS *= $${CRT_v142_X64_RELEASE_DLLS}
}
