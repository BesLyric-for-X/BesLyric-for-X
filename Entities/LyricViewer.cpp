#include "LyricViewer.h"
#include "global.h"
#include <QFileInfo>
#include <QRegExp>
#include <BesMessageBox.h>
#include <QSpacerItem>
#include "LrcProcessor.h"
#include "BesScaleUtil.h"

LyricViewer::LyricViewer(QWidget* parent):QWidget(parent)
{
    setMouseTracking(true);//详见 BesFramelessWidget.h 注释

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

    labelPreviewSongTip->setMaximumSize(80,30 * BesScaleUtil::mscale());
    labelPreviewLyricTip->setMaximumSize(80,30 * BesScaleUtil::mscale());
    labelPreviewSongTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelPreviewLyricTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    labelPreviewSongPath = new QLabel(this);
    labelPreviewLyricPath = new QLabel(this);
    labelPreviewSongPath->setObjectName("labelPreviewSongPath");
    labelPreviewLyricPath->setObjectName("labelPreviewLyricPath");
    labelPreviewSongPath->setText("无");
    labelPreviewLyricPath->setText("无");

    labelPreviewSongPath->setAlignment(Qt::AlignLeft);
    labelPreviewLyricPath->setAlignment(Qt::AlignLeft);

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelPreviewSongTip);
    hLayout1->addWidget(labelPreviewSongPath);

    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(labelPreviewLyricTip);
    hLayout2->addWidget(labelPreviewLyricPath);

    scrollLyricPanel = new ScrollLyricPanel(this);
    scrollLyricPanel->setObjectName("scrollLyricPanel");
    scrollLyricPanel->setMinimumSize(548* BesScaleUtil::scale(),500* BesScaleUtil::scale());
    scrollLyricPanel->setMaximumSize(548* BesScaleUtil::scale(),500* BesScaleUtil::scale());
    scrollLyricPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addSpacerItem(new QSpacerItem(10,10* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addWidget(labelPreviewTitle);
    vLayout->addSpacerItem(new QSpacerItem(10,10* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout1);
    vLayout->addSpacerItem(new QSpacerItem(10,5* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(10,10* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::Fixed));
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
    labelPreviewTitle->setToolTip(title);
}

void LyricViewer::setMusicPath(QString path)
{
    labelPreviewSongPath->setText(path);
    labelPreviewSongPath->setToolTip(path);
}

void LyricViewer::setLrcLyricPath(QString path)
{
    labelPreviewLyricPath->setText(path);
    labelPreviewLyricPath->setToolTip(path);

    //使用该路径初始化歌词预览面板
    if(!loadLyricFromFile(path))
    {
        BesMessageBox::information(tr("提示"),tr("打开载入lrc歌词时发生错误：")+path
                                   +"\n\n"+ tr("请注意是否歌词是否存在？"));

        lrcLyrics.clear();
        bIsLrcLyric = false;
    }

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

