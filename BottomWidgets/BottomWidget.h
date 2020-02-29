#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>
#include <QLabel>
#include "BesSlider.h"
#include "BesButton.h"

class MusicPlayer;

class BottomWidget : public QWidget
{
    Q_OBJECT

public:
    BottomWidget(QWidget *parent = 0);
    ~BottomWidget();

    void initLayout();
    void initEntity();
    void initConnection();

public slots:
    void reloadMusic(QString musicPath);
    void play();
    void pause();
    void playFromBegin();               //从开头开始播放
    void autoPlayOrPause();             //自动判断播放还是暂停
    void stop();                        //【特别注意：本程序的播放器不是线程安全的，在stop()中的循环等待播放线程结束操作，
                                        // 可能会由于别处同时 调用 stop(),play() 而卡死】
    void seek(qint64 pos);              //毫秒时间
    void seekForward(qint64 step);
    void seekBackward(qint64 step);


    void enterMakingMode();     //进入制作模式
    void exitMakingMode();      //退出制作模式

    void positionChanged(int);

public:
    void setCurrentPlayMode(int mode);  //歌曲播放模式   0、单曲播放 1、单曲循环

private slots:
    void onPlayOrPause();
    void durationChanged(qint64);

    void onSliderSongMoved(int position);
    void onSliderSongPressed();
    void onSliderSongReleased();
    void onSliderSongClickNotOnHandle(int position);

    void onSoundToggle(bool);
    void onSliderSoundChanged(int);
    void onSliderSoundPressed();
    void onSliderSoundReleased();

    void onModeSingleNext();
    void onModeSingleCycleNext();

    void onErrorOccurs(int code,QString strErr);

    void onAudioFinished(bool isEndWithForce);
    void onAudioPlay();
    void onAudioPause();

private:
    void showPosition(int position);  //预览位置，在调整拖动时使用

private:
    bool bInMakingMode;     //标记是否在制作模式中

    int nVolumeBeforeMute;  //静音前的音量
    bool bSliderSoundPress;

public:
    BesButton*        btnPreSong;
    BesButton*        btnPlayAndPause;
    BesButton*        btnNextSong;

    QLabel*           labelTimeCurrent;
    QLabel*           labelTimeEnding;

    BesSlider*        sliderSong;
    QSlider*          sliderSound;

    BesButton*        btnSound;
    BesButton*        btnPlayModeSingle;            //单曲播放
    BesButton*        btnPlayModeSingleCycle;       //单曲循环播放
    BesButton*        btnDesktopLyric;

    MusicPlayer*      musicPlayer;

private:

    bool AdjustingPos;
    qint64 posAdjust;
};

#endif // BOTTOMWIDGET_H
