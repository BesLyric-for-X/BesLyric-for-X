#include "BoxPageLyricList.h"
#include <QVBoxLayout>

BoxPageLyricList::BoxPageLyricList(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
    initLayout();
    initConnection();
}

BoxPageLyricList::~BoxPageLyricList()
{

}

void BoxPageLyricList::initLayout()
{
    widgetMainLyricList = new QWidget(this);
    widgetMainLyricList->setObjectName("widgetMainLyricList");
    widgetMainLyricList->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    extendButton = new ExtendButton(widgetMainLyricList);

    labelLyricListBoxTip = new QLabel(widgetMainLyricList);
    labelLyricListBoxTip->setText(tr("歌词单"));
    labelLyricListBoxTip->setObjectName("labelLyricListBoxTip");

    QVBoxLayout* layoutMain = new QVBoxLayout(this);
    layoutMain->setMargin(0);
    layoutMain->addWidget(widgetMainLyricList);
}

void BoxPageLyricList::initConnection()
{

}

void BoxPageLyricList::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QRect leftWidgetRect = QRect(5 ,5,this->height()-2*5, this->height()-2*5);
    extendButton->setGeometry(leftWidgetRect);

    QRect rightWidgetRect = leftWidgetRect;
    rightWidgetRect.setLeft(leftWidgetRect.left() + leftWidgetRect.width() + 10);
    rightWidgetRect.setRight(this->width() - leftWidgetRect.width() -10);
    labelLyricListBoxTip->setGeometry(rightWidgetRect);
}


void BoxPageLyricList::changePic(QPixmap pic)
{
    extendButton->SetPixmap(pic);
}


void BoxPageLyricList::setToDefaultPic()
{
    extendButton->SetToDefaultPixmap();
}

void BoxPageLyricList::setFinalSkinName(QString skinName)
{
    if(skinName == "black")
        extendButton->SetPixmap(QPixmap(":/resource/image/box_lyric_list_black.png"));
    else
        extendButton->SetPixmap(QPixmap(":/resource/image/box_lyric_list_white.png"));
}
