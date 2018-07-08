#include "SubPageMaking.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPixmap>

SubPageMaking::SubPageMaking(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
    initLayout();
    connectAll();
}

SubPageMaking::~SubPageMaking()
{

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
    labelSelectMusic->setMinimumSize(100,25);
    labelSelectLyric->setMinimumSize(100,25);
    labelSelectOutputDir->setMinimumSize(100,25);
    labelSelectMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSelectLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSelectOutputDir->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    editSelectMusic = new QLineEdit(this);
    editSelectLyric = new QLineEdit(this);
    editSelectOutputDir = new QLineEdit(this);
    editSelectMusic->setMinimumHeight(25);
    editSelectLyric->setMinimumHeight(25);
    editSelectOutputDir->setMinimumHeight(25);
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
    btnSelectMusic->setMinimumSize(100,25);
    btnSelectLyric->setMinimumSize(100,25);
    btnSelectOutputDir->setMinimumSize(100,25);
    btnSelectMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSelectLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSelectOutputDir->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    QHBoxLayout* hLayout2 = new QHBoxLayout();
    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout1->addWidget(labelSelectMusic);
    hLayout1->addWidget(editSelectMusic);
    hLayout1->addWidget(btnSelectLyric);
    hLayout2->addWidget(labelSelectLyric);
    hLayout2->addWidget(editSelectLyric);
    hLayout2->addWidget(btnSelectMusic);
    hLayout3->addWidget(labelSelectOutputDir);
    hLayout3->addWidget(editSelectOutputDir);
    hLayout3->addWidget(btnSelectOutputDir);

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    vLayout->addSpacerItem(new QSpacerItem(20,30,QSizePolicy::Minimum,QSizePolicy::Minimum));

    labelTip= new QLabel(this);
    labelTipUp= new QLabel(this);
    labelTipSpace= new QLabel(this);
    labelTipBack= new QLabel(this);
    labelTipReturn= new QLabel(this);
    labelTip->setText(tr("提示："));
    labelTipUp->setText(tr("推上一行"));
    labelTipSpace->setText(tr("回退5秒"));
    labelTipBack->setText(tr("暂停"));
    labelTipReturn->setText(tr("结束"));
    labelTip->setMinimumHeight(25);
    labelTipUp->setMinimumHeight(25);
    labelTipSpace->setMinimumHeight(25);
    labelTipBack->setMinimumHeight(25);
    labelTipReturn->setMinimumHeight(25);
    labelTip->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipUp->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipSpace->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipBack->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    labelTipReturn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    btnLoadLastFiles = new BesButton(this);
    btnLoadLastFiles->setText(tr("载入最新"));
    btnLoadLastFiles->setMinimumHeight(25);
    btnLoadLastFiles->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    QLabel *imageLabel1 = new QLabel(this);
    QLabel *imageLabel2 = new QLabel(this);
    QLabel *imageLabel3 = new QLabel(this);
    QLabel *imageLabel4 = new QLabel(this);
    imageLabel1->setMinimumHeight(25);
    imageLabel2->setMinimumHeight(25);
    imageLabel3->setMinimumHeight(25);
    imageLabel4->setMinimumHeight(25);

    imageLabel1->setPixmap(QPixmap(":/resource/image/key_up.png"));
    imageLabel2->setPixmap(QPixmap(":/resource/image/key_space.png"));
    imageLabel3->setPixmap(QPixmap(":/resource/image/key_back.png"));
    imageLabel4->setPixmap(QPixmap(":/resource/image/key_return.png"));

    QHBoxLayout* hLayout4 = new QHBoxLayout();
    hLayout4->addWidget(labelTip);
    hLayout4->addWidget(imageLabel1);
    hLayout4->addWidget(labelTipUp);
    hLayout4->addWidget(imageLabel2);
    hLayout4->addWidget(labelTipSpace);
    hLayout4->addWidget(imageLabel3);
    hLayout4->addWidget(labelTipBack);
    hLayout4->addWidget(imageLabel4);
    hLayout4->addWidget(labelTipReturn);
    hLayout4->addSpacerItem(new QSpacerItem(20,25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout4->addWidget(btnLoadLastFiles);
    vLayout->addLayout(hLayout4);

    widgetCurrentMusicAndLyric = new QWidget(this);  //歌曲和歌词显示控件

    labelCurrentMusicTip= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentMusic= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentLyricTip= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentLyric= new QLabel(widgetCurrentMusicAndLyric);
    labelCurrentMusicTip->setText(tr("当前音乐："));
    labelCurrentLyricTip->setText(tr("当前歌词："));
    labelCurrentMusicTip->setMinimumSize(100,25);
    labelCurrentLyricTip->setMinimumSize(100,25);
    labelCurrentMusic->setMinimumHeight(25);
    labelCurrentLyric->setMinimumHeight(25);
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

    labelTimeTip= new QLabel(widgetLyricBoard);
    labelCurrenLineTip= new QLabel(widgetLyricBoard);
    labelNextLineTip= new QLabel(widgetLyricBoard);
    labelTimeTip->setText(tr("00:00.000"));
    labelCurrenLineTip->setText(tr("当前行"));
    labelNextLineTip->setText(tr("下一行："));
    labelTimeTip->setMinimumSize(100,25);
    labelCurrenLineTip->setMinimumSize(100,25);
    labelNextLineTip->setMinimumSize(100,25);
    labelTimeTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelCurrenLineTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelNextLineTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    widgetMiddleLine = new QWidget(widgetLyricBoard);

    labelLine1= new QLabel(widgetLyricBoard);
    labelLine2= new QLabel(widgetMiddleLine);
    labelLine3= new QLabel(widgetLyricBoard);
    labelLine1->setMinimumHeight(25);
    labelLine2->setMinimumHeight(25);
    labelLine3->setMinimumHeight(25);
    labelLine1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelLine2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelLine3->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    labelCurrenLineEmptyTip = new QLabel(widgetMiddleLine);
    labelCurrenLineEmptyTip->setText(tr("空行"));
    labelCurrenLineEmptyTip->setMinimumSize(80,25);
    labelCurrenLineEmptyTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


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
    vBoxLayout2->addLayout(hLayout8);
    vBoxLayout2->addLayout(hLayout9);
    vBoxLayout2->addLayout(hLayout10);

    vLayout->addWidget(widgetLyricBoard);

    btnPreviewResult = new BesButton(this);
    btnOpenResult = new BesButton(this);
    btnToRemaking = new BesButton(this);
    btnStartMaking = new BesButton(this);
    btnPreviewResult->setText(tr("预览效果"));
    btnOpenResult->setText(tr("打开生成文件"));
    btnToRemaking->setText(tr("我要重制"));
    btnStartMaking->setText(tr("开始制作"));
    btnPreviewResult->setMinimumHeight(25);
    btnOpenResult->setMinimumHeight(25);
    btnToRemaking->setMinimumHeight(25);
    btnStartMaking->setMinimumHeight(25);
    QHBoxLayout* hLayout11 = new QHBoxLayout();
    hLayout11->addWidget(btnPreviewResult);
    hLayout11->addWidget(btnOpenResult);
    hLayout11->addSpacerItem(new QSpacerItem(20,25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout11->addWidget(btnToRemaking);
    hLayout11->addWidget(btnStartMaking);
    vLayout->addLayout(hLayout11);
    vLayout->addSpacerItem(new QSpacerItem(20,30,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding));

}

void SubPageMaking::connectAll()
{

}
