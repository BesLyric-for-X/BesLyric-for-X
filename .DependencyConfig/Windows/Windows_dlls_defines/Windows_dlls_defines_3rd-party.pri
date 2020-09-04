# ------3rd party------


FFMPEG_DLLS = \
    avcodec-58.dll \
    avdevice-58.dll \
    avfilter-7.dll \
    avformat-58.dll \
    avutil-56.dll \
    postproc-55.dll \
    swresample-3.dll \
    swscale-5.dll

SDL2_DLLS = \
    SDL2.dll

OPENSSL_X86_DLLS = \
    libcrypto-1_1.dll \
    libssl-1_1.dll

OPENSSL_X64_DLLS = \
    libcrypto-1_1-x64.dll \
    libssl-1_1-x64.dll

equals(QT_ARCH, i386) {
    OPENSSL_DLLS = $${OPENSSL_X86_DLLS}
} else:equals(QT_ARCH, x86_64) {
    OPENSSL_DLLS = $${OPENSSL_X64_DLLS}
}

THIRD_PARTY_DLLS = $${FFMPEG_DLLS} $${SDL2_DLLS} $${OPENSSL_DLLS}
