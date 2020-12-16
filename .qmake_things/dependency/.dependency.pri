# OS-specific dependency configurations.


win32 {
    include(windows/.windows.pri)
}
unix {
    # The order is key.
    macx {
        include(macos/.macos.pri)
    }
    include(linux/.linux.pri)
}
