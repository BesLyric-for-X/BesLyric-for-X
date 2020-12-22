# ------ MSVC UCRT ------


UCRT_COMMON_DLLS = \
    api-ms-win-core-console-l1-1-0.dll \
    api-ms-win-core-console-l1-2-0.dll \
    api-ms-win-core-datetime-l1-1-0.dll \
    api-ms-win-core-debug-l1-1-0.dll \
    api-ms-win-core-errorhandling-l1-1-0.dll \
    api-ms-win-core-file-l1-1-0.dll \
    api-ms-win-core-file-l1-2-0.dll \
    api-ms-win-core-file-l2-1-0.dll \
    api-ms-win-core-handle-l1-1-0.dll \
    api-ms-win-core-heap-l1-1-0.dll \
    api-ms-win-core-interlocked-l1-1-0.dll \
    api-ms-win-core-libraryloader-l1-1-0.dll \
    api-ms-win-core-localization-l1-2-0.dll \
    api-ms-win-core-memory-l1-1-0.dll \
    api-ms-win-core-namedpipe-l1-1-0.dll \
    api-ms-win-core-processenvironment-l1-1-0.dll \
    api-ms-win-core-processthreads-l1-1-0.dll \
    api-ms-win-core-processthreads-l1-1-1.dll \
    api-ms-win-core-profile-l1-1-0.dll \
    api-ms-win-core-rtlsupport-l1-1-0.dll \
    api-ms-win-core-string-l1-1-0.dll \
    api-ms-win-core-synch-l1-1-0.dll \
    api-ms-win-core-synch-l1-2-0.dll \
    api-ms-win-core-sysinfo-l1-1-0.dll \
    api-ms-win-core-timezone-l1-1-0.dll \
    api-ms-win-core-util-l1-1-0.dll \
    api-ms-win-crt-conio-l1-1-0.dll \
    api-ms-win-crt-convert-l1-1-0.dll \
    api-ms-win-crt-environment-l1-1-0.dll \
    api-ms-win-crt-filesystem-l1-1-0.dll \
    api-ms-win-crt-heap-l1-1-0.dll \
    api-ms-win-crt-locale-l1-1-0.dll \
    api-ms-win-crt-math-l1-1-0.dll \
    api-ms-win-crt-multibyte-l1-1-0.dll \
    api-ms-win-crt-private-l1-1-0.dll \
    api-ms-win-crt-process-l1-1-0.dll \
    api-ms-win-crt-runtime-l1-1-0.dll \
    api-ms-win-crt-stdio-l1-1-0.dll \
    api-ms-win-crt-string-l1-1-0.dll \
    api-ms-win-crt-time-l1-1-0.dll \
    api-ms-win-crt-utility-l1-1-0.dll

UCRT_x86_COMMON_DLLS = \
    API-MS-Win-core-xstate-l2-1-0.dll

equals(QT_ARCH, i386) {
    UCRT_COMMON_DLLS *= $${UCRT_x86_COMMON_DLLS}
}

UCRT_DEBUG_DLLS = \
    ucrtbased.dll

UCRT_RELEASE_DLLS = \
    ucrtbase.dll
