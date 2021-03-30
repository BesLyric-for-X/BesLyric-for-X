<img alt="BesLyric-for-X" src="Beslyric.ico" width=32px>

[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg?style=flat-square)](https://996.icu/#/zh_CN)
[![Latest version](https://img.shields.io/badge/dynamic/json?color=blue&label=latest%20version&style=flat-square&query=%24.tag_name&url=https%3A%2F%2Fapi.github.com%2Frepos%2FBesLyric-for-X%2FBesLyric-for-X%2Freleases%2Flatest)](https://github.com/BesLyric-for-X/BesLyric-for-X/releases/latest)
[![Gitter](https://img.shields.io/badge/Gitter-BesLyric--for--X__org%2Fcommunity-%2346BC99?style=flat-square)](https://gitter.im/BesLyric-for-X_org/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![BesLyric QQ 群](https://img.shields.io/badge/QQ%20%E7%BE%A4-1021317114-red?style=flat-square)](https://shang.qq.com/wpa/qunwpa?idkey=90548f8500d6f5b5fd9b6ee89684206053b709b6309a0dc807cdb4cd8704a78e)

# Beslyric-for-X  

Beslyric-for-X ，是原 [Beslyric](https://github.com/BensonLaur/BesLyric) 的跨平台升级版，基于QT实现。

Beslyric 目的在于制作一款 操作简单、功能实用的 专门用于制作网易云音乐滚动歌词的 歌词制作软件。基于Qt实现，主打歌词制作功能，以网易云风格界面，力图为云村村民提供一个良好的歌词制作体验！

BesLyric-for-X 详细介绍与下载，访问 [这里](http://www.cnblogs.com/BensonLaur/p/9695769.html)

## 简单展示 

windows:

<img src="https://github.com/BensonLaur/image-storage/raw/master/beslyric-for-x/beslyric-for-x-demo-1.png" 
            width="70%" height="70%">
            
<img src="https://github.com/BensonLaur/image-storage/raw/master/beslyric-for-x/beslyric-for-x-demo-2.png" 
            width="70%" height="70%">

Linux(Debian/Ubuntu):

<img src="https://github.com/BensonLaur/image-storage/raw/master/beslyric-for-x/beslyric-for-x-demo-3.png" 
            width="70%" height="70%">
            
MacOs:

<img src="https://github.com/BensonLaur/image-storage/raw/master/beslyric-for-x/beslyric-for-x-demo-4.png" 
            width="70%" height="70%">

## 开发说明

#### 开发环境

IDE: [QT Creator 5.7.1](https://download.qt.io/archive/qt/5.7/5.7.1/)  

#### 开发准备

1、本项目为项目主体，clone 本项目到本地使用 Qt Creator 开发

2、本项目使用跨平台开源库 ffmpeg 解析播放音频文件，考虑到更新ffmpeg的灵活性 以及 跨平台要求的特性，Beslyric-for-X 中在使用 ffmpeg 时，不直接将其置于项目下，而是开发者在对应的平台上各自独立单独安装。具体开发说明置于 [beslyric-lib](https://github.com/BensonLaur/beslyric-lib) 项目中。

