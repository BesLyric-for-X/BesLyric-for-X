#include "LyricViewer.h"
#include "global.h"

LyricViewer::LyricViewer(QWidget* parent):QWidget(parent)
{
    setMouseTracking(true);

    initLayout();
    initConnection();
}

void LyricViewer::initLayout()
{
    labelPreviewTitle = new QLabel(this);
    labelPreviewSongTip= new QLabel(this);
    labelPreviewLyricTip= new QLabel(this);
    labelPreviewTitle->setObjectName("labelPreviewTitle");
    labelPreviewSongTip->setObjectName("labelPreviewSongTip");
    labelPreviewLyricTip->setObjectName("labelPreviewLyricTip");
    labelPreviewTitle->setText("我们的歌");
    labelPreviewSongTip->setText("歌曲路径");
    labelPreviewLyricTip->setText("歌词路径");

    scrollLyricPanel = new ScrollLyricPanel(this);
    scrollLyricPanel->setMinimumSize(500,500);
    scrollLyricPanel->setMaximumSize(500,500);
    scrollLyricPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(labelPreviewTitle);
    vLayout->addWidget(labelPreviewSongTip);
    vLayout->addWidget(labelPreviewLyricTip);
    vLayout->addWidget(scrollLyricPanel);
}

void LyricViewer::initConnection()
{



}
