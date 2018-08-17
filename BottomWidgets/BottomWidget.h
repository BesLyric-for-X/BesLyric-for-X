#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
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
    void stop();
    void seek(quint64 pos);             //毫秒时间
    void seekForward(quint64 step);
    void seekBackward(quint64 step);


    void enterMakingMode();     //进入制作模式
    void exitMakingMode();      //退出制作模式

    void positionChanged(int);

private slots:
    void durationChanged(qint64);
    void volumeChanged(int);

    void onSliderSongMoved(int position);
    void onSliderSongPressed();
    void onSliderSongReleased();

private:
    bool bInMakingMode;     //标记是否在制作模式中

public:
    BesButton*        btnPreSong;
    BesButton*        btnPlayAndPause;
    BesButton*        btnNextSong;

    QLabel*           labelTimeCurrent;
    QLabel*           labelTimeEnding;

    QSlider*          sliderSong;
    QSlider*          sliderSound;

    BesButton*        btnSound;
    BesButton*        btnDesktopLyric;
    BesButton*        btnPlayMode;

    MusicPlayer*      musicPlayer;

private:

    bool AdjustingPos;
    quint64 posAdjust;
};

#endif // BOTTOMWIDGET_H
