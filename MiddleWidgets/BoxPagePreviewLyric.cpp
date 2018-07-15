#include "BoxPagePreviewLyric.h"
#include <QVBoxLayout>

BoxPagePreviewLyric::BoxPagePreviewLyric(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    initLayout();
    connectAll();
}

BoxPagePreviewLyric::~BoxPagePreviewLyric()
{

}

void BoxPagePreviewLyric::initLayout()
{
    widgetMainPreviewLyric = new QWidget(this);
    widgetMainPreviewLyric->setObjectName("widgetMainPreviewLyric");

    extendButton = new ExtendButton(widgetMainPreviewLyric);

    QVBoxLayout* layoutMain = new QVBoxLayout(this);
    layoutMain->setMargin(0);
    layoutMain->addWidget(widgetMainPreviewLyric);

}

void BoxPagePreviewLyric::connectAll()
{

}

void BoxPagePreviewLyric::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QRect leftWidgetRect = QRect(5 ,5,this->height()-2*5, this->height()-2*5);

    extendButton->setGeometry(leftWidgetRect);
}
