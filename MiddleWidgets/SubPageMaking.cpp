#include "SubPageMaking.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QFileDialog>
#include <BesMessageBox.h>
#include <BesShadowDialog.h>
#include <QDesktopServices>

SubPageMaking::SubPageMaking(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);

    initLayout();
    initEntity();
    initConnection();
}

SubPageMaking::~SubPageMaking()
{

}

void SubPageMaking::initEntity()
{
    pathLoaded = false;
    pathResultLrcLyric = "";
    isMaking = false;

    btnPreviewResult->setEnabled(false);
    btnOpenResult->setEnabled(false);
    btnToRemaking->setEnabled(false);
    btnStartMaking->setEnabled(false);
}

void SubPageMaking::initLayout()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);

    labelSelectMusic = new QLabel(this);
    labelSelectLyric= new QLabel(this);
    labelSelectOutputDir= new QLabel(this);
    labelSelectMusic->setText(tr("选择音乐："));
    labelSelectLyric->setText(tr("选择歌词："));
    labelSelectOutputDir->setText(tr("输出目录："));
    labelSelectMusic->setMinimumSize(100,28);
    labelSelectLyric->setMinimumSize(100,28);
    labelSelectOutputDir->setMinimumSize(100,28);
    labelSelectMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSelectLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSelectOutputDir->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    editSelectMusic = new QLineEdit(this);
    editSelectLyric = new QLineEdit(this);
    editSelectOutputDir = new QLineEdit(this);
    editSelectMusic->setMinimumHeight(28);
    editSelectLyric->setMinimumHeight(28);
    editSelectOutputDir->setMinimumHeight(28);
    editSelectMusic->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editSelectLyric->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editSelectOutputDir->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editSelectMusic->setPlaceholderText(tr("点击选择文件文件 或 拖放文件到这里"));
    editSelectLyric->setPlaceholderText(tr("点击选择文件文件 或 拖放文件到这里"));
    editSelectOutputDir->setPlaceholderText(tr("点击选择文件文件 或 拖放文件到这里"));

    btnSelectMusic = new BesButton(this);
    btnSelectLyric = new BesButton(this);
    btnSelectOutputDir = new BesButton(this);
    btnSelectMusic->setText(tr("选择"));
    btnSelectLyric->setText(tr("选择"));
    btnSelectOutputDir->setText(tr("选择"));
    btnSelectMusic->setMinimumSize(100,28);
    btnSelectLyric->setMinimumSize(100,28);
    btnSelectOutputDir->setMinimumSize(100,28);
    btnSelectMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSelectLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSelectOutputDir->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    QHBoxLayout* hLayout2 = new QHBoxLayout();
    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout1->addWidget(labelSelectMusic);
    hLayout1->addWidget(editSelectMusic);
    hLayout1->addWidget(btnSelectMusic);
    hLayout2->addWidget(labelSelectLyric);
    hLayout2->addWidget(editSelectLyric);
    hLayout2->addWidget(btnSelectLyric);
    hLayout3->addWidget(labelSelectOutputDir);
    hLayout3->addWidget(editSelectOutputDir);
    hLayout3->addWidget(btnSelectOutputDir);

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    vLayout->addSpacerItem(new QSpacerItem(20,30,QSizePolicy::Minimum,QSizePolicy::Minimum));

    labelTip= new QLabel(this);
    labelTipUp= new QLabel(this);
    labelTipEmpty =new QLabel(this);
    labelTipSpace= new QLabel(this);
    labelTipBack= new QLabel(this);
    labelTipReturn= new QLabel(this);
    labelTip->setText(tr("提示："));
    labelTipUp->setText(tr("推上一行"));
    labelTipEmpty->setText(tr("空出一行"));
    labelTipBack->setText(tr("回退5秒"));
    labelTipSpace->setText(tr("暂停"));
    labelTipReturn->setText(tr("结束"));

    labelTip->setMinimumHeight(28);
    labelTipUp->setMinimumHeight(28);
    labelTipEmpty->setMinimumHeight(28);
    labelTipSpace->setMinimumHeight(28);
    labelTipBack->setMinimumHeight(28);
    labelTipReturn->setMinimumHeight(28);

    labelTip->setMaximumWidth(150);
    labelTipUp->setMaximumWidth(150);
    labelTipEmpty->setMaximumWidth(150);
    labelTipSpace->setMaximumWidth(150);
    labelTipBack->setMaximumWidth(150);
    labelTipReturn->setMaximumWidth(150);

    labelTip->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipUp->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipSpace->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipBack->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipReturn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    btnLoadLastFiles = new BesButton(this);
    btnLoadLastFiles->setText(tr("载入最新"));
    btnLoadLastFiles->setMinimumHeight(28);
    btnLoadLastFiles->setMinimumWidth(100);
    btnLoadLastFiles->setMaximumWidth(200);
    btnLoadLastFiles->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    QLabel *imageLabel1 = new QLabel(this);
    QLabel *imageLabel2 = new QLabel(this);
    QLabel *imageLabel3 = new QLabel(this);
    QLabel *imageLabel4 = new QLabel(this);
    QLabel *imageLabel5 = new QLabel(this);
    imageLabel1->setMinimumHeight(28);
    imageLabel2->setMinimumHeight(28);
    imageLabel3->setMinimumHeight(28);
    imageLabel4->setMinimumHeight(28);
    imageLabel5->setMinimumHeight(28);

    imageLabel1->setMaximumWidth(30);
    imageLabel2->setMaximumWidth(30);
    imageLabel3->setMaximumWidth(30);
    imageLabel4->setMaximumWidth(50);
    imageLabel5->setMaximumWidth(30);

    imageLabel1->setPixmap(QPixmap(":/resource/image/key_up.png"));
    imageLabel2->setPixmap(QPixmap(":/resource/image/key_right.png"));
    imageLabel3->setPixmap(QPixmap(":/resource/image/key_back.png"));
    imageLabel4->setPixmap(QPixmap(":/resource/image/key_space.png"));
    imageLabel5->setPixmap(QPixmap(":/resource/image/key_return.png"));

    imageLabel1->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel3->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel4->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    imageLabel5->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QHBoxLayout* hLayout4 = new QHBoxLayout();
    QHBoxLayout* hLayout4_5 = new QHBoxLayout();

    hLayout4->addWidget(labelTip);
    hLayout4->addSpacerItem(new QSpacerItem(10,28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel1);
    hLayout4->addWidget(labelTipUp);
    hLayout4->addSpacerItem(new QSpacerItem(10,28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel2);
    hLayout4->addWidget(labelTipEmpty);
    hLayout4->addSpacerItem(new QSpacerItem(10,28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel3);
    hLayout4->addWidget(labelTipBack);
    hLayout4->addSpacerItem(new QSpacerItem(10,28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel4);
    hLayout4->addWidget(labelTipSpace);
    hLayout4->addSpacerItem(new QSpacerItem(10,28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4->addWidget(imageLabel5);
    hLayout4->addWidget(labelTipReturn);
    hLayout4->addSpacerItem(new QSpacerItem(20,28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4_5->addSpacerItem(new QSpacerItem(300,28,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout4_5->addWidget(btnLoadLastFiles);
    hLayout4->addLayout(hLayout4_5);
    vLayout->addLayout(hLayout4);

    widgetCurrentMusicAndLyric = new QWidget(this);  //歌曲和歌词显示控件

    labelCurrentMusicTip= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentMusic= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentLyricTip= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentLyric= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentMusicTip->setText(tr("当前音乐："));
    labelCurrentLyricTip->setText(tr("当前歌词："));
    labelCurrentMusicTip->setMinimumSize(100,28);
    labelCurrentLyricTip->setMinimumSize(100,28);
    labelCurrentMusic->setMinimumHeight(28);
    labelCurrentLyric->setMinimumHeight(28);
    labelCurrentMusicTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrentLyricTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrentMusic->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelCurrentLyric->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    QHBoxLayout* hLayout5 = new QHBoxLayout();  //注意：布局定义属于整个父窗口
    QHBoxLayout* hLayout6 = new QHBoxLayout();
    hLayout5->addWidget(labelCurrentMusicTip);
    hLayout5->addWidget(labelCurrentMusic);
    hLayout6->addWidget(labelCurrentLyricTip);
    hLayout6->addWidget(labelCurrentLyric);

    QVBoxLayout* vBoxLayout1 = new QVBoxLayout(widgetCurrentMusicAndLyric);
    vBoxLayout1->addLayout(hLayout5);
    vBoxLayout1->addLayout(hLayout6);

    vLayout->addWidget(widgetCurrentMusicAndLyric);

    widgetLyricBoard = new QWidget(this);   //具体歌词显示板
    widgetLyricBoard->setObjectName("widgetLyricBoard");

    labelTimeTip= new QLabel(widgetLyricBoard);
    labelCurrenLineTip= new QLabel(widgetLyricBoard);
    labelNextLineTip= new QLabel(widgetLyricBoard);
    labelTimeTip->setText(tr("00:00.000"));
    labelCurrenLineTip->setText(tr("当前行："));
    labelNextLineTip->setText(tr("下一行："));
    labelTimeTip->setMinimumSize(100,28);
    labelCurrenLineTip->setMinimumSize(100,28);
    labelNextLineTip->setMinimumSize(100,28);
    labelTimeTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrenLineTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelNextLineTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    widgetMiddleLine = new QWidget(widgetLyricBoard);

    labelLine1= new QLabel(widgetLyricBoard);
    labelLine2= new QLabel(widgetMiddleLine);
    labelLine3= new QLabel(widgetLyricBoard);
    labelLine1->setMinimumHeight(28);
    labelLine2->setMinimumHeight(28);
    labelLine3->setMinimumHeight(28);
    labelLine1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelLine2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelLine3->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    labelLine1->setObjectName("labelLine1");
    labelLine2->setObjectName("labelLine2");
    labelLine3->setObjectName("labelLine3");

    labelCurrenLineEmptyTip = new QLabel(widgetMiddleLine);
    labelCurrenLineEmptyTip->setText(tr("空行"));
    labelCurrenLineEmptyTip->setMinimumSize(80,25);
    labelCurrenLineEmptyTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrenLineEmptyTip->setObjectName("labelCurrenLineEmptyTip");
    labelCurrenLineEmptyTip->setAlignment(Qt::AlignCenter);
    labelCurrenLineEmptyTip->setVisible(false);


    QHBoxLayout* hLayout7 = new QHBoxLayout(widgetLyricBoard);
    hLayout7->addWidget(labelLine2);
    hLayout7->addWidget(labelCurrenLineEmptyTip);
    widgetMiddleLine->setLayout(hLayout7);

    QHBoxLayout* hLayout8 = new QHBoxLayout();  //注意：布局定义属于整个父窗口
    QHBoxLayout* hLayout9 = new QHBoxLayout();
    QHBoxLayout* hLayout10 = new QHBoxLayout();
    hLayout8->addWidget(labelTimeTip);
    hLayout8->addWidget(labelLine1);
    hLayout9->addWidget(labelCurrenLineTip);
    hLayout9->addWidget(widgetMiddleLine);
    hLayout10->addWidget(labelNextLineTip);
    hLayout10->addWidget(labelLine3);

    QVBoxLayout* vBoxLayout2 = new QVBoxLayout(widgetLyricBoard);
    vBoxLayout2->addSpacerItem(new QSpacerItem(20,9,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vBoxLayout2->addLayout(hLayout8);
    vBoxLayout2->addLayout(hLayout9);
    vBoxLayout2->addLayout(hLayout10);
    vBoxLayout2->addSpacerItem(new QSpacerItem(20,9,QSizePolicy::Fixed,QSizePolicy::Fixed));

    vLayout->addWidget(widgetLyricBoard);

    btnPreviewResult = new BesButton(this);
    btnOpenResult = new BesButton(this);
    btnToRemaking = new BesButton(this);
    btnStartMaking = new BesButton(this);
    btnPreviewResult->setText(tr("预览效果"));
    btnOpenResult->setText(tr("打开生成文件"));
    btnToRemaking->setText(tr("我要重制"));
    btnStartMaking->setText(tr("开始制作"));
    btnPreviewResult->setMinimumSize(100,28);
    btnOpenResult->setMinimumSize(100,28);
    btnToRemaking->setMinimumSize(100,28);
    btnStartMaking->setMinimumSize(100,28);
    QHBoxLayout* hLayout11 = new QHBoxLayout();
    hLayout11->addWidget(btnPreviewResult);
    hLayout11->addWidget(btnOpenResult);
    hLayout11->addSpacerItem(new QSpacerItem(20,28,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout11->addWidget(btnToRemaking);
    hLayout11->addWidget(btnStartMaking);
    vLayout->addSpacerItem(new QSpacerItem(20,30,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout11);
    vLayout->addSpacerItem(new QSpacerItem(20,30,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding));

}

void SubPageMaking::initConnection()
{
    connect(btnSelectMusic, SIGNAL(clicked(bool)),this,SLOT(selectMusicPath()));
    connect(btnSelectLyric, SIGNAL(clicked(bool)),this,SLOT(selectLyricPath()));
    connect(btnSelectOutputDir, SIGNAL(clicked(bool)),this,SLOT(selectOutputDir()));
    connect(btnLoadLastFiles, SIGNAL(clicked(bool)),this,SLOT(loadCurrentPath()));

    connect(btnStartMaking, SIGNAL(clicked(bool)),this,SLOT(startMaking()));
    connect(btnToRemaking,SIGNAL(clicked(bool)),this,SLOT(remaking()));

    connect(btnPreviewResult,SIGNAL(clicked(bool)),this,SLOT(previewResult()));
    connect(btnOpenResult,SIGNAL(clicked(bool)),this,SLOT(openResult()));
}

//推上一行
void SubPageMaking::markOneLine()
{
    if(lyricMaker.hasNextRawLine())
        lyricMaker.markCurrentRawLine(curAudioPos);

    updateLinesText();
}

//空出一行
void SubPageMaking::markEmptyLine()
{
    lyricMaker.markEmptyLine(curAudioPos);
    updateLinesText();
}

//回退5秒
void SubPageMaking::backBy5Second()
{
    emit onSeekBackward(5000);

    quint64 to = 0;
    if(curAudioPos > 5000)
        to = curAudioPos - 5000;

    lyricMaker.stepBackToTime(to);
    updateLinesText();
}

//暂停或开始
void SubPageMaking::playOrPause()
{
    emit onPlayOrPauseMusic();
}

//结束制作
void SubPageMaking::finishMaking()
{
    if(isMaking == true)
    {
        isMaking = false;

        lyricMaker.finishMaking();

        if(lyricMaker.isResultLrcEmpty())
        {
            BesMessageBox::information( tr("提示"),tr("制作结果为空，请重新按提示制作"));
        }
        else
        {
            QFileInfo musicFile(pathMusic);
            QString outputFile = pathOutputDir + "/" + musicFile.baseName() + ".lrc";

            if(lyricMaker.saveLyrc(outputFile))
            {
                pathResultLrcLyric = outputFile;
                BesMessageBox::information(tr("提示"),tr("成功保存到：")+outputFile);
            }
            else
            {
                BesMessageBox::information( tr("提示"),tr("成功失败，无法保存到：")+outputFile);
            }
        }

        emit onStopMusic();

        emit onExitMakingMode();

        btnLoadLastFiles->setEnabled(true);
        btnPreviewResult->setEnabled(true);
        btnOpenResult->setEnabled(true);
        btnToRemaking->setEnabled(false);
    }
}

//点击了预览效果
void SubPageMaking::previewResult()
{
    if(pathResultLrcLyric == "")
        return;

    if(!pathLoaded) //路径都没有载入
        return;

    //载入lrc歌词，并且播放当前歌曲来预览
    emit loadLrcLyricAndSwitchToPreview(pathResultLrcLyric);

}

void SubPageMaking::openResult()
{
    QFileInfo fileInfo(pathResultLrcLyric);
    QString dirPath = fileInfo.dir().absolutePath();
    QDesktopServices::openUrl(QUrl("file:///"+dirPath, QUrl::TolerantMode));
    QDesktopServices::openUrl(QUrl("file:///"+pathResultLrcLyric, QUrl::TolerantMode));
}

//更新当前行内容的显示
void SubPageMaking::updateLinesText()
{
    QString line1, line2, line3;
    lyricMaker.getPreLrcLineText(line1);

    bool bRet = lyricMaker.getCurrentLrcLineText(line2);    //为了能够显示空行，先尝试获取lrc
    if(!bRet)
        lyricMaker.getCurrentRawLineText(line2);  //一开始未制作时，lrc为空，获取失败，获取raw
    else
    {
        //获取成功，看是否为空，为空显示空行提示
        if(line2.size() == 0)
            labelCurrenLineEmptyTip->setVisible(true);
        else
            labelCurrenLineEmptyTip->setVisible(false);
    }

    lyricMaker.getNextRawLineText(line3);

    labelLine1->setText(line1);
    labelLine2->setText(line2);
    labelLine3->setText(line3);
}



void SubPageMaking::selectMusicPath()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开音频文件"),
                                                      "/home",
                                                      tr("音频 (*.mp3 *.wav);;视频 (*.mp4)"));

    if(fileName.size() !=0)
    {
        editSelectMusic->setText(fileName);
        pathMusic = fileName;
    }
}

void SubPageMaking::selectLyricPath()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("打开原歌词文件"), "/home",
                                                      tr("文本 (*.txt);;其他 (*.*)"));
    if(fileName.size() !=0)
    {
        editSelectLyric->setText(fileName);
        pathLyric = fileName;
    }
}

void SubPageMaking::selectOutputDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("打开 LRC 歌词输出目录"),  "/home",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(dir.size() !=0)
    {
        editSelectOutputDir->setText(dir);
        pathOutputDir = dir;
    }
}

void SubPageMaking::loadCurrentPath()
{
    //检测路径是否都选择完毕
    if(pathMusic.size() == 0)
    {
        BesMessageBox::information( tr("提示"),tr("请先选择音乐"));
        return;
    }

    if(pathLyric.size() == 0)
    {
        BesMessageBox::information( tr("提示"),tr("请先选择歌词"));
        return;
    }

    if(pathOutputDir.size() == 0)
    {
        BesMessageBox::information( tr("提示"),tr("请先选择输出目录"));
        return;
    }

    //载入歌词到歌词制作器
    if(!lyricMaker.loadRawLyric(pathLyric))
    {
        BesMessageBox::information( tr("提示"),tr("读取歌词失败"));
        return;
    }

    if(lyricMaker.isRawLyricEmpty())
    {
        BesMessageBox::information( tr("提示"),tr("歌词数据为空，请重新载入歌词文件"));
        return;
    }

    pathLoaded = true;

    emit onReloadMusic(pathMusic); 

    //初始化歌词制作过程
    initMakingProcess();

    btnStartMaking->setEnabled(true);
    btnPreviewResult->setEnabled(false);
    btnOpenResult->setEnabled(false);
}


void SubPageMaking::startMaking()
{
    if(!pathLoaded)
        return; //路径没有成功加载，不响应

    if(isMaking == false)  //没有在制作时才允许响应制作的操作
    {
        isMaking = true;

        emit onStartMaking();

        emit onEnterMakingMode();

        btnStartMaking->setEnabled(false);
        btnLoadLastFiles->setEnabled(false);
        btnToRemaking->setEnabled(true);
    }
}


void SubPageMaking::remaking()
{
    if(!pathLoaded)
        return; //路径没有成功加载，不响应

    if(isMaking == true)  //在制作时才允许响应制作的操作
    {
        isMaking = false;  //必须先置为 false ，因为音乐的停止会触发 finishMaking 操作

        emit onStopMusic();

        emit onExitMakingMode();

        //初始化歌词制作过程
        initMakingProcess();

        btnToRemaking->setEnabled(false);
        btnStartMaking->setEnabled(true);
        btnLoadLastFiles->setEnabled(true);
    }
}

void SubPageMaking::updatePos(int pos)
{
	if (isMaking)
	{
		curAudioPos = pos;

		//计算时间字符串

		int ms = pos % 1000;
		pos = pos / 1000;
		int s = pos % 60;
		int m = pos / 60;

		QString timeLabel;
		timeLabel.sprintf("%.2d:%.2d.%.3d", m, s, ms);

		labelTimeTip->setText(timeLabel);
	}
}

//初始化制作歌词的过程
void SubPageMaking::initMakingProcess()
{
    if(pathLoaded)
    {
        labelTimeTip->setText("00:00.000");

        //更新当前选中的路径提示
        labelCurrentMusic->setText(pathMusic);
        labelCurrentLyric->setText(pathLyric);

        //获取数据填充到行上
        lyricMaker.startMaking();

        QString firstLine;
        lyricMaker.getNextRawLineText(firstLine);

        labelLine1->setText("");
        labelLine2->setText("");
        labelLine3->setText(firstLine);
        labelCurrenLineEmptyTip->setVisible(false);
    }
}
