# Basic tests


# Distinguish the architectures.
#   Don't use "QMAKE_HOST.arch" because it reported "x86" when the 64 bit kit enabled. https://bugreports.qt.io/browse/QTBUG-30263
equals(QT_ARCH, i386) {
    message("QT_ARCH = x86")
} else:equals(QT_ARCH, x86_64) {
    message("QT_ARCH = x64")
} else {
    error("Unknown architecture")
}
