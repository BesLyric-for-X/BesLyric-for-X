#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include "BesButton.h"

class BottomWidget : public QWidget
{
    Q_OBJECT

public:
    BottomWidget(QWidget *parent = 0);
    ~BottomWidget();

    void initLayout();
    void connectAll();

public:
    BesButton*        btnPreSong;
    BesButton*        btnPlayAndPause;
    BesButton*        btnNextSong;

    QLabel*             labelTimeCurrent;
    QLabel*             labelTimeEnding;

    QSlider*            sliderSong;
    QSlider*            sliderSound;

    BesButton*        btnSound;
    BesButton*        btnDesktopLyric;
    BesButton*        btnPlayMode;
};

#endif // BOTTOMWIDGET_H
