<img alt="BesLyric-for-X" src="Beslyric.ico" width=32px>

[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg?style=flat-square)](https://996.icu/#/zh_CN)
[![Latest version](https://img.shields.io/badge/dynamic/json?color=blue&label=latest%20version&style=flat-square&query=%24.tag_name&url=https%3A%2F%2Fapi.github.com%2Frepos%2FBesLyric-for-X%2FBesLyric-for-X%2Freleases%2Flatest)](https://github.com/BesLyric-for-X/BesLyric-for-X/releases/latest)
[![Gitter](https://img.shields.io/badge/Gitter-BesLyric--for--X__org%2Fcommunity-%2346BC99?style=flat-square)](https://gitter.im/BesLyric-for-X_org/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![BesLyric QQ 群](https://img.shields.io/badge/QQ%20%E7%BE%A4-1021317114-red?style=flat-square)](https://shang.qq.com/wpa/qunwpa?idkey=90548f8500d6f5b5fd9b6ee89684206053b709b6309a0dc807cdb4cd8704a78e)

# BesLyric-for-X

## 简介

BesLyric-for-X 是一款操作简单、功能实用的歌词制作软件。云村的村民们能够在网易云风格的界面中获得良好的歌词制作体验。

本软件的前身是 [BesLyric](https://github.com/BesLyric-for-X/BesLyric) ，它只能运行在 Windows 中。为了 Linux 和 macOS 的用户，我们基于 Qt 框架开发了 BesLyric-for-X 。

经过测试，本软件可在下列操作系统中正常运行：

- Linux （ 64 位）
  - Ubuntu 18.04
- macOS （ 64 位）
  - 10.13
  - 10.14
  - 10.15
- Windows （ 32 / 64 位）
  - 7 (6.1.7600)
  - 10

请访问[这篇博文](http://www.cnblogs.com/BensonLaur/p/9695769.html)查看使用教程或可能遇到的问题。

BesLyric-for-X 所使用的程序图标来自 [Netease Cloudmusic icons - OPPO Color OS Icons - Easyicon](https://www.easyicon.net/1186871-netease_cloudmusic_icon.html) 。

## 开发说明

### 获取源代码

```console
$ git clone --recurse-submodules https://github.com/BesLyric-for-X/BesLyric-for-X.git
$ #         \--------__--------/
$ #              Important!
```

### 环境

框架：

- Qt 5.12.4+

操作系统：

- Linux ([Qt for Linux/X11 | Qt 5 § Supported Configurations](https://doc.qt.io/qt-5/linux.html#supported-configurations))
  - Ubuntu 18.04+
  - Fedora release 32
  - Manjaro Linux xfce 20.0.3
  - openSUSE Leap 15.2
- macOS 10.14 & 10.15 ([Qt for macOS | Qt 5 § Supported Versions](https://doc.qt.io/qt-5/macos.html#supported-versions))
- Windows 10 ([Qt for Windows | Qt 5 § Supported Configurations](https://doc.qt.io/qt-5/windows.html#supported-configurations))

工具链：

- Linux: GCC 5+
- macOS: Xcode 11+ (Command Line Tools (CLT) only)
- Windows
  - MSVC 2017+
  - MinGW 7.3.0+

本项目依赖这些第三方库：

- FFmpeg 4
- SDL 2
- OpenSSL 1.1.1

### 构建

请按照 [BesLyric-for-X/BesLyric-for-X_Conf](https://github.com/BesLyric-for-X/BesLyric-for-X_Conf) 进行开发环境的配置，再进行构建。

#### Linux 或 macOS

```console
$ qmake && make
```

#### Windows

```console
> qmake -before "B4X_DEP_PATH=C:\b4x-lib"

> nmake         # MSVC
> mingw32-make  # MinGW
```
