#ifndef SUBPAGEMAKING_H
#define SUBPAGEMAKING_H

#include "global.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "BesFileLineEdit.h"

#include "BesButton.h"
#include "LyricMaker.h"
#include "ThreadGuessLyricInfo.h"

#include "LyricEditorBox.h"

class SubPageMaking : public QWidget
{
    Q_OBJECT

public:
    SubPageMaking(QWidget *parent = 0);
    ~SubPageMaking();

    void initLayout();
    void initEntity();
    void initConnection();

    void markOneLine();     //推上一行
    void markEmptyLine();   //空出一行
    void backBy5Second();   //回退5秒
    void backOneLine();     //回退到上一行

    void playOrPause();     //暂停或开始
    void pause();           //暂停

private:
    void updateLinesText(); //更新当前行内容的显示

signals:
    void onReloadMusic(QString musicPath);
    void onStartMaking();
    void onSeekBackward(qint64);           //回退指定毫秒时间
    void onPlayOrPauseMusic();
    void onPauseMusic();
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
    void finishMaking();              //结束制作
    void previewResult();             //点击了预览效果
    void openResult();                //打开生成文件

    void updatePos(qint64);

    void onGuessNcmInfo();            //猜测ncm文件的歌曲名和歌词
    void onGuessLyricInfo();          //猜测歌词信息

    void onEditCurrentRawLyric();     //直接打开文件编辑当前原歌词
    void onEditCurrentLine();         //编辑当前行
    void onEditBatchLyric();          //批量编辑

    //默认输出路径发生改变
    void OnDefaultPathOutputChanged(QString path);

public:
    void selectMusicPath(const QString& musicPath);
    void selectLyricPath(const QString& lyricPath);
    void selectOutputPath(const QString& outputPath);

    void toggleMiddleLineEdit(bool showEdit);            //是否切换出中间行编辑
private:
    void initMakingProcess(bool updateCurrentSongLyric); //初始化制作歌词的过程
    void toggleMusicAndLyricPath(bool bExtend);          //切换是否显示路径

public:
    bool isMaking;									//标记是否正在制作
    bool isEditing;                                 //标记是否正在编辑
    bool isBatchEditing;                            //标记是否正在批量编辑
	ThreadGuessLyricInfo	threadGuessLyricInfo;	//当前猜词用的线程

private:
    QString         pathMusic;          //当前选择中的路径
    QString         pathLyric;
    QString         pathOutputDir;

    LyricMaker      lyricMaker;
    qint64          curAudioPos;        //当前歌曲播放到的位置

    bool            pathLoaded;         //标记路径是否被成功加载
    QString         pathMusicLoaded;    //当前被载入的音乐
    QString         pathResultLrcLyric; //生成的结果


public:
    QLabel*         labelSelectMusic;
    QLabel*         labelSelectLyric;
    QLabel*         labelSelectOutputDir;

    BesFileLineEdit*  editSelectMusic;
    BesFileLineEdit*  editSelectLyric;
    BesFileLineEdit*  editSelectOutputDir;

    BesButton*    btnSelectMusic;
    BesButton*    btnSelectLyric;
    BesButton*    btnSelectOutputDir;

    BesButton*    btnDownloadMp3;
    BesButton*    btnGuessLyricInfo;
    BesButton*    btnEditTxtLyric;

    QWidget*      widgetMusicPath;
    QWidget*      widgetLyricPath;

    QLabel*         labelTip;
    QLabel*         labelTipUp;
    QLabel*         labelTipEmpty;
    QLabel*         labelTipSpace;
    QLabel*         labelTipBack;
    QLabel*         labelTipDown;
    QLabel*         labelTipReturn;

    BesButton*    btnLoadLastFiles;

    QWidget*        widgetCurrentMusicAndLyric;

    QLabel*         labelCurrentMusicTip;
    QLabel*         labelCurrentMusic;
    QLabel*         labelCurrentLyricTip;
    QLabel*         labelCurrentLyric;

    QWidget*        widgetLyricBoard;

    BesButton*      btnEditLyricCurrent;
    QLabel*         labelTimeTip;
    QLabel*         labelCurrenLineTip;
    QLabel*         labelNextLineTip;
    BesButton*      btnEditBatchLyric;

    QLabel*         labelLine0;
    QLabel*         labelLine1;
    QLabel*         labelLine2;
    QLabel*         labelLine3;
    QLabel*         labelLine4;
    QLineEdit*      editMiddleLine;
    QWidget*        widgetLine0;
    QWidget*        widgetMiddleLine;
    QWidget*        widgetLine4;
    QLabel*         labelCurrenLineEmptyTip;

    QLabel*         labelDoneMark0;
    QLabel*         labelDoneMark1;
    QLabel*         labelDoneMark2;
    QSpacerItem*    middleSpacer;

    BesButton*    btnPreviewResult;
    BesButton*    btnOpenResult;
    BesButton*    btnToRemaking;
    BesButton*    btnStartMaking;

};

#endif // SUBPAGEMAKING_H
