# HomeBrew and pkg-config. Tested on macOS 10.13, 10.14 and 10.15


# pkg-config is needed.

# But, `/usr/local/bin` is not in `${PATH}` on Qt started from Finder,
#   so `qmake` will not be able to find the `pkg-config` installed using HomeBrew.
# Obviously, it is useless to just set `${PKG_CONFIG_PATH}`.


# Environment variable `B4X_PKG_CONFIG` may be needed.
#   For `B4X_PKG_CONFIG`: ASCII, no accents, spaces nor symlinks, short path, and point to `pkg-config`.


B4X_PKG_CONFIG = $$getenv(B4X_PKG_CONFIG)

isEmpty(B4X_PKG_CONFIG) {
    B4X_PKG_CONFIG = /usr/local/bin/pkg-config # I give you a default value.
    message("env \"B4X_PKG_CONFIG\" is NOT set, try \"$${B4X_PKG_CONFIG}\".")
}

contains(B4X_PKG_CONFIG, "^\s*$") {
    error("\"B4X_PKG_CONFIG\" is empty.")
}

B4X_PKG_CONFIG = $$system_path($$absolute_path($$clean_path($${B4X_PKG_CONFIG})))
message(B4X_PKG_CONFIG = $${B4X_PKG_CONFIG})
!exists($${B4X_PKG_CONFIG}) {
    error("\"$${B4X_PKG_CONFIG}\" is NOT existed.")
}

PKG_CONFIG = $${B4X_PKG_CONFIG}

# More config will be done by Linux-common.pri
