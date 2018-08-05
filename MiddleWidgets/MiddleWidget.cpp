#include "MiddleWidget.h"

MiddleWidget::MiddleWidget(QWidget *parent)
    : QWidget(parent),currentPage(0)
{
    this->setMouseTracking(true);
    initLayout();
    initAnimation();
    initConnection();
}

MiddleWidget::~MiddleWidget()
{

}

void MiddleWidget::initLayout()
{
    pageMain = new PageMain(this);
    pagePreviewLyric = new PagePreviewLyric(this);
    pageMatchNcmID = new PageMatchNcmID(this);
    pageLyricList = new PageLyricList(this);
    pageSetting = new PageSetting(this);

    pageMain->raise();
}

void MiddleWidget::initConnection()
{
    connect(pageMain->boxPagePreviewLyric->extendButton, &QPushButton::clicked, [=](){
        switchPage(0,1); });

    connect(pagePreviewLyric->btnPackupLyricBox, &QPushButton::clicked,[=](){
        switchPage(1,0); });

    connect(&animationPreviewExtend, SIGNAL(finished()), this, SLOT(geometryAnimationFinish()) );
}


void MiddleWidget::initAnimation()
{
    animationPreviewExtend.setPropertyName("geometry");
    animationPreviewExtend.setDuration(100);

}


void MiddleWidget::AnimateFromRectToRect(QWidget* widget, QRect fromRect, QRect toRect)
{
    widget->raise();
    animationPreviewExtend.setTargetObject(widget);
    animationPreviewExtend.setStartValue(fromRect);
    animationPreviewExtend.setEndValue(toRect);
    animationPreviewExtend.start();

}


void MiddleWidget::switchPage(int from, int to)
{
    if(from == 0 && to == 1)
    {
        QPoint pt = pageMain->boxPagePreviewLyric->extendButton->pos();
        QPoint GlobalPoint(pageMain->boxPagePreviewLyric->extendButton->mapTo(this,pt));//获取控件在窗体中的坐标

        QRect previewBoxRect = pageMain->boxPagePreviewLyric->extendButton->geometry();
        previewBoxRect = previewBoxRect.translated(GlobalPoint);


        AnimateFromRectToRect(pagePreviewLyric,previewBoxRect,QRect(0,0,this->width(),this->height()));
    }

    if(from == 1 && to == 0)
    {

        QPoint pt = pageMain->boxPagePreviewLyric->extendButton->pos();
        QPoint GlobalPoint(pageMain->boxPagePreviewLyric->extendButton->mapTo(this,pt));//获取控件在窗体中的坐标

        QRect previewBoxRect = pageMain->boxPagePreviewLyric->extendButton->geometry();
        previewBoxRect = previewBoxRect.translated(GlobalPoint);

        AnimateFromRectToRect(pagePreviewLyric,QRect(0,0,this->width(),this->height()),previewBoxRect);

    }

    currentPage = to;
}


void MiddleWidget::geometryAnimationFinish()
{
    switch(currentPage)
    {
    case 0: pageMain->raise();   break;
    case 1: pagePreviewLyric->raise();   break;
    case 2: pageLyricList->raise();   break;
    case 3: pageMatchNcmID->raise();   break;
    case 4: pageSetting->raise();   break;
    }
}


void MiddleWidget::resizeEvent(QResizeEvent *event)
{
    pageMain->setGeometry(0,0,this->width(),this->height());
    pagePreviewLyric->setGeometry(0,0,this->width(),this->height());
    pageLyricList->setGeometry(0,0,this->width(),this->height());
    pageMatchNcmID->setGeometry(0,0,this->width(),this->height());
    pageSetting->setGeometry(0,0,this->width(),this->height());

    switch(currentPage)
    {
    case 0: pageMain->raise();   break;
    case 1: pagePreviewLyric->raise();   break;
    case 2: pageLyricList->raise();   break;
    case 3: pageMatchNcmID->raise();   break;
    case 4: pageSetting->raise();   break;
    }

    QWidget::resizeEvent(event);
}

