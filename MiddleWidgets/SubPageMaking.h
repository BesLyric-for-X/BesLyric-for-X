#ifndef SUBPAGEMAKING_H
#define SUBPAGEMAKING_H

#include "global.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>

#include "BesButton.h"
#include "LyricMaker.h"
#include "ThreadGuessLyricInfo.h"

class SubPageMaking : public QWidget
{
    Q_OBJECT

public:
    SubPageMaking(QWidget *parent = 0);
    ~SubPageMaking();

    void initEntity();
    void initLayout();
    void initConnection();


    void markOneLine();     //推上一行
    void markEmptyLine();   //空出一行
    void backBy5Second();   //回退5秒
    void playOrPause();     //暂停或开始

private:
    void updateLinesText(); //更新当前行内容的显示


signals:
    void onReloadMusic(QString musicPath);
    void onStartMaking();
    void onSeekBackward(quint64);           //回退指定毫秒时间
    void onPlayOrPauseMusic();
    void onStopMusic();

    void onEnterMakingMode();
    void onExitMakingMode();

    //载入lrc歌词，并且播放当前歌曲来预览
    void loadLrcLyricAndSwitchToPreview(QString lycLyricPath);

    void sig_addToMakingHistory(QString musicPath, QString lrcPath);

public slots:

    void selectMusicPath();
    void selectLyricPath();
    void selectOutputDir();
    void loadCurrentPath();
    void startMaking();
    void remaking();
    void finishMaking();    //结束制作
    void previewResult();   //点击了预览效果
    void openResult();      //打开生成文件

    void updatePos(int);

	void onGuessLyricInfo();  //猜测歌词信息

public:
    void selectMusicPath(const QString& musicPath);
    void selectLyricPath(const QString& lyricPath);
    void selectOutputPath(const QString& outputPath);


private:
    void initMakingProcess(); //初始化制作歌词的过程

public:
    bool isMaking;									//标记是否正在制作
	ThreadGuessLyricInfo	threadGuessLyricInfo;	//当前猜词用的线程

private:
    QString         pathMusic;          //当前选择中的路径
    QString         pathLyric;
    QString         pathOutputDir;

    LyricMaker      lyricMaker;
    int             curAudioPos;        //当前歌曲播放到的位置

    bool            pathLoaded;         //标记路径是否被成功加载
    QString         pathMusicLoaded;    //当前被载入的音乐
    QString         pathResultLrcLyric; //生成的结果


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

    BesButton*    btnGuessLyricInfo;

    QLabel*         labelTip;
    QLabel*         labelTipUp;
    QLabel*         labelTipEmpty;
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
