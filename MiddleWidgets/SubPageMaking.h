#ifndef SUBPAGEMAKING_H
#define SUBPAGEMAKING_H

#include "global.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "BesButton.h"

class SubPageMaking : public QWidget
{
    Q_OBJECT

public:
    SubPageMaking(QWidget *parent = 0);
    ~SubPageMaking();

    void initLayout();
    void connectAll();

public:
    QLabel*         labelSelectMusic;
    QLabel*         labelSelectLyric;
    QLabel*         labelSelectOutputDir;

    QLineEdit*      editSelectMusic;
    QLineEdit*      editSelectLyric;
    QLineEdit*      editSelectOutputDir;

    BesButton*    btnSelectMusic;
    BesButton*    btnSelectLyric;
    BesButton*    btnSelectOutputDir;

    QLabel*         labelTip;
    QLabel*         labelTipUp;
    QLabel*         labelTipSpace;
    QLabel*         labelTipBack;
    QLabel*         labelTipReturn;

    BesButton*    btnLoadLastFiles;

    QWidget*        widgetCurrentMusicAndLyric;

    QLabel*         labelCurrentMusicTip;
    QLabel*         labelCurrentMusic;
    QLabel*         labelCurrentLyricTip;
    QLabel*         labelCurrentLyric;

    QWidget*        widgetLyricBoard;

    QLabel*         labelTimeTip;
    QLabel*         labelCurrenLineTip;
    QLabel*         labelNextLineTip;

    QLabel*         labelLine1;
    QLabel*         labelLine2;
    QLabel*         labelLine3;
    QWidget*        widgetMiddleLine;
    QLabel*         labelCurrenLineEmptyTip;

    BesButton*    btnPreviewResult;
    BesButton*    btnOpenResult;
    BesButton*    btnToRemaking;
    BesButton*    btnStartMaking;

};

#endif // SUBPAGEMAKING_H
