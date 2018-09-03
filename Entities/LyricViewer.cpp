#include "LyricViewer.h"
#include "global.h"
#include <QFileInfo>
#include <QRegExp>
#include <BesMessageBox.h>
#include <QSpacerItem>
#include "LrcProcessor.h"

LyricViewer::LyricViewer(QWidget* parent):QWidget(parent)
{
    setMouseTracking(true);

    initEntity();
    initLayout();
    initConnection();
}


void LyricViewer::initEntity()
{
    lrcLyrics.clear();
    bIsLrcLyric = false;
    bIsLyricValid = false;
    bIsNeteaseLrcFormat = true;
}

void LyricViewer::initLayout()
{
    labelPreviewTitle = new QLabel(this);
    labelPreviewSongTip= new QLabel(this);
    labelPreviewLyricTip= new QLabel(this);
    labelPreviewTitle->setObjectName("labelPreviewTitle");
    labelPreviewSongTip->setObjectName("labelPreviewSongTip");
    labelPreviewLyricTip->setObjectName("labelPreviewLyricTip");
    labelPreviewTitle->setText("");
    labelPreviewSongTip->setText("歌曲路径:");
    labelPreviewLyricTip->setText("歌词路径:");

    labelPreviewSongTip->setMaximumSize(80,30);
    labelPreviewLyricTip->setMaximumSize(80,30);
    labelPreviewSongTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelPreviewLyricTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    labelPreviewSongPath = new QLabel(this);
    labelPreviewLyricPath = new QLabel(this);
    labelPreviewSongPath->setObjectName("labelPreviewSongPath");
    labelPreviewLyricPath->setObjectName("labelPreviewLyricPath");
    labelPreviewSongPath->setText("无");
    labelPreviewLyricPath->setText("无");

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelPreviewSongTip);
    hLayout1->addWidget(labelPreviewSongPath);

    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(labelPreviewLyricTip);
    hLayout2->addWidget(labelPreviewLyricPath);

    scrollLyricPanel = new ScrollLyricPanel(this);
    scrollLyricPanel->setObjectName("scrollLyricPanel");
    scrollLyricPanel->setMinimumSize(548,500);
    scrollLyricPanel->setMaximumSize(548,500);
    scrollLyricPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addWidget(labelPreviewTitle);
    vLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout1);
    vLayout->addSpacerItem(new QSpacerItem(10,5,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addWidget(scrollLyricPanel);
}

void LyricViewer::initConnection()
{



}

//歌词是否有效(路径不存在或者为空都是无效)
bool  LyricViewer::isLyricValid()
{
    return bIsLyricValid;
}

//歌词是否是LRC歌词
bool LyricViewer::isLrcLyric()
{
    return bIsLrcLyric;
}

void LyricViewer::setMusicTitle(QString title)
{
    labelPreviewTitle->setText(title);
}

void LyricViewer::setMusicPath(QString path)
{
    QFileInfo fileinfo(path);
    setMusicTitle(fileinfo.baseName());     //先使用歌曲的文件的名字，来设置音乐标题，如果播放时，从歌曲中发现了标题，那么再由外界设置新标题

    labelPreviewSongPath->setText(path);
}

void LyricViewer::setLrcLyricPath(QString path)
{
    labelPreviewLyricPath->setText(path);

    //使用该路径初始化歌词预览面板
    if(!loadLyricFromFile(path))
        BesMessageBox::information(tr("提示"),tr("载入lrc歌词时发生错误：")+path);

    scrollLyricPanel->updateLyric(lrcLyrics, bIsLrcLyric);
}

//设置空的歌词
void LyricViewer::setEmptyLyric()
{
    labelPreviewLyricPath->setText(tr("无"));

    lrcLyrics.clear();
    scrollLyricPanel->updateLyric(lrcLyrics, true);

    bIsLyricValid = false;
}


void LyricViewer::skinNameChanged(QString skinName)
{
    if(skinName == "black")
        scrollLyricPanel->lyricPanel->switchColor(true);
    else
       scrollLyricPanel->lyricPanel->switchColor(false);
}


bool LyricViewer::loadLyricFromFile(QString lyricPath)
{
    LrcProcessor lrcProcessor;

    if(!lrcProcessor.LoadFromFile(lyricPath))
    {
        return false;
    }
    else
    {
        lrcLyrics = lrcProcessor.GetLrcLyric();

        bIsLyricValid = lrcProcessor.bIsLrcLyric;
        bIsLrcLyric = lrcProcessor.bIsLrcLyric;
        bIsNeteaseLrcFormat = lrcProcessor.bIsNeteaseLrcFormat;
    }

    return true;
}

