# Main file about OS-specific dependencies configuration.


# Basic test

# Distinguish the architectures of the Windows platform.
# Don't use "QMAKE_HOST.arch" because it reported "x86" when the 64 bit kit enabled. https://bugreports.qt.io/browse/QTBUG-30263
equals(QT_ARCH, i386) {
    message("QT_ARCH = x86")
} else:equals(QT_ARCH, x86_64) {
    message("QT_ARCH = x64")
} else {
    error("Unknown architecture")
}


# Determine the OS.

win32 {
    include(Windows/Windows_common.pri)
}
unix {
    # The order is key.
    macx {
        include(macOS/macOS_common.pri)
    }
    include(Linux/Linux_common.pri)
}
