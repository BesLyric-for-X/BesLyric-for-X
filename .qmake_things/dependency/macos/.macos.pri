# macOS 10.13, 10.14 and 10.15.


# Because the path of HomeBrew prefix (that is `/usr/local` by default) may not be included by the environment variable `PATH` in Finder,
#   qmake will not be able to find "pkg-config" installed by HomeBrew.
# So, just setting `PKG_CONFIG_PATH` to help "pkg-config" to find "*.pc" files is useless in many cases.


# The environment variable `B4X_PKG_CONFIG` is optional.
#   For `B4X_PKG_CONFIG`: ASCII, no accents, spaces nor symlinks, short path, and points to the binary "pkg-config".


B4X_PKG_CONFIG = $$getenv(B4X_PKG_CONFIG)

isEmpty(B4X_PKG_CONFIG) {
    B4X_PKG_CONFIG = /usr/local/bin/pkg-config # I give you a default value.
    message("env \"B4X_PKG_CONFIG\" is NOT set, try \"$${B4X_PKG_CONFIG}\".")
}

contains(B4X_PKG_CONFIG, "^\s*$") {
    error("\"B4X_PKG_CONFIG\" is empty.")
}

B4X_PKG_CONFIG = $$system_path($$absolute_path($$clean_path($${B4X_PKG_CONFIG})))
message("B4X_PKG_CONFIG = $${B4X_PKG_CONFIG}")
!exists($${B4X_PKG_CONFIG}) {
    error("\"$${B4X_PKG_CONFIG}\" is NOT existed.")
}

PKG_CONFIG = $${B4X_PKG_CONFIG}

# More configurations will be done by "Linux_common.pri".
