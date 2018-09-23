#include "BoxPagePreviewLyric.h"
#include <QVBoxLayout>

BoxPagePreviewLyric::BoxPagePreviewLyric(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initLayout();
    initConnection();
}

BoxPagePreviewLyric::~BoxPagePreviewLyric()
{

}

void BoxPagePreviewLyric::initLayout()
{
    widgetMainPreviewLyric = new QWidget(this);
    widgetMainPreviewLyric->setObjectName("widgetMainPreviewLyric");
    widgetMainPreviewLyric->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    extendButton = new ExtendButton(widgetMainPreviewLyric);

    labelTitle = new QLabel(widgetMainPreviewLyric);
    labelArtist = new QLabel(widgetMainPreviewLyric);

    QVBoxLayout* layoutMain = new QVBoxLayout(this);
    layoutMain->setMargin(0);
    layoutMain->addWidget(widgetMainPreviewLyric);

}

void BoxPagePreviewLyric::initConnection()
{

}

void BoxPagePreviewLyric::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int picOuterWidth = this->height();
    int picWidth = this->height()-2*5;
    QRect leftWidgetRect = QRect(5 ,5, picWidth, this->height()-2*5);

    extendButton->setGeometry(leftWidgetRect);

    QRect labelRect1 = QRect(picOuterWidth + 5 ,5, this->width() - picOuterWidth- 2*5, this->height()/2 - 5);
    QRect labelRect2 = QRect(picOuterWidth + 5 ,this->height()/2 ,
                             this->width() - picOuterWidth- 2*5, this->height()/2 - 5);

    labelTitle->setGeometry(labelRect1);
    labelArtist->setGeometry(labelRect2);
}


void BoxPagePreviewLyric::changePic(QPixmap pic)
{
    extendButton->SetPixmap(pic);
}


void BoxPagePreviewLyric::setToDefaultPic()
{
    extendButton->SetToDefaultPixmap();
}

void BoxPagePreviewLyric::onSetSongTitle(QString title)
{
    labelTitle->setText(title);
    labelTitle->setToolTip(title);
}

void BoxPagePreviewLyric::onSetSongArtist(QString artist)
{
    labelArtist->setText(artist);
    labelArtist->setToolTip(artist);
}
