#include "MiddleWidget.h"

MiddleWidget::MiddleWidget(QWidget *parent)
    : QWidget(parent),pageMain(nullptr),pagePreviewLyric(nullptr),pageLyricList(nullptr),
      pageMatchNcmID(nullptr),pageSetting(nullptr),currentPage(0)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
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

    connect(pageMain->boxPageLyricList->extendButton, &QPushButton::clicked, [=](){
        switchPage(0,2); });

//    connect(boxPageLyricList->btnPackupLyricBox, &QPushButton::clicked,[=](){
//        switchPage(2,0); });

    connect(&animationPreviewExtend, SIGNAL(finished()), this, SLOT(geometryAnimationFinish()) );
    connect(&animationLrcListExtend, SIGNAL(finished()), this, SLOT(geometryAnimationFinish()) );
    connect(&animationSettingExtend, SIGNAL(finished()), this, SLOT(geometryAnimationFinish()) );

    connect(pageMain->subPageMaking, SIGNAL(sig_addToMakingHistory(QString,QString)),
            pageLyricList, SLOT(OnAddToMakingHistory(QString,QString)));
   }


void MiddleWidget::initAnimation()
{
    //由于一个动画对象不能同时驱动2个动画，所以每个页面对应一个动画对象
    animationPreviewExtend.setPropertyName("geometry");
    animationLrcListExtend.setPropertyName("geometry");
    animationSettingExtend.setPropertyName("geometry");

    nRuningAnimationCount = 0;
}


void MiddleWidget::AnimateFromRectToRect(QPropertyAnimation& animation, QWidget* widget, QRect fromRect, QRect toRect)
{
    nRuningAnimationCount ++;
    widget->raise();
    animation.setDuration(100 * nRuningAnimationCount); //同时多个动画时，后面的动画时间间隔100加长
    animation.setTargetObject(widget);
    animation.setStartValue(fromRect);
    animation.setEndValue(toRect);
    animation.start();
}


void MiddleWidget::switchPage(int from, int to)
{
    if(from == 0 && to == 1)
    {
        QPoint pt = pageMain->boxPagePreviewLyric->extendButton->pos();
        QPoint GlobalPoint(pageMain->boxPagePreviewLyric->extendButton->mapTo(this,pt));//获取控件在窗体中的坐标

        QRect previewBoxRect = pageMain->boxPagePreviewLyric->extendButton->geometry();
        previewBoxRect = previewBoxRect.translated(GlobalPoint);


        AnimateFromRectToRect(animationPreviewExtend,pagePreviewLyric,
                              previewBoxRect,QRect(0,0,this->width(),this->height()));
    }

    if(from == 1 && to == 0)
    {
        QPoint pt = pageMain->boxPagePreviewLyric->extendButton->pos();
        QPoint GlobalPoint(pageMain->boxPagePreviewLyric->extendButton->mapTo(this,pt));//获取控件在窗体中的坐标

        QRect previewBoxRect = pageMain->boxPagePreviewLyric->extendButton->geometry();
        previewBoxRect = previewBoxRect.translated(GlobalPoint);

        AnimateFromRectToRect(animationPreviewExtend,pagePreviewLyric,
                              QRect(0,0,this->width(),this->height()),previewBoxRect);

    }

    if(from == 0 && to == 2)
    {
        QPoint pt = pageMain->boxPageLyricList->extendButton->pos();
        QPoint GlobalPoint(pageMain->boxPageLyricList->extendButton->mapTo(this,pt));//获取控件在窗体中的坐标

        QRect BoxRect = pageMain->boxPageLyricList->extendButton->geometry();
        BoxRect = BoxRect.translated(GlobalPoint);


        AnimateFromRectToRect(animationLrcListExtend,pageLyricList,
                              BoxRect,QRect(0,0,this->width(),this->height()));
    }

    if(from == 2 && to == 0)
    {
        QPoint pt = pageMain->boxPageLyricList->extendButton->pos();
        QPoint GlobalPoint(pageMain->boxPageLyricList->extendButton->mapTo(this,pt));//获取控件在窗体中的坐标

        QRect BoxRect = pageMain->boxPageLyricList->extendButton->geometry();
        BoxRect = BoxRect.translated(GlobalPoint);

        AnimateFromRectToRect(animationLrcListExtend,pageLyricList,
                              QRect(0,0,this->width(),this->height()),BoxRect);

    }

    if(from == 0 && to == 4)
    {
        QRect rectUnderSetting(this->width() - 200, 0, this->width()-190,10);
        AnimateFromRectToRect(animationSettingExtend,pageSetting,
                              rectUnderSetting, QRect(0,0,this->width(),this->height()));
    }

    if(from == 4 && to == 0)
    {
        QRect rectUnderSetting(this->width() - 200, 0, this->width()-190,10);
        AnimateFromRectToRect(animationSettingExtend,pageSetting,
                              QRect(0,0,this->width(),this->height()),rectUnderSetting);
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
    case 4:
        pageMain->raise();      //由于setting出现前可能会有1、2页面存在，其动画目标（currentPage）为 0，
                                //  而在currentPage 被setting 页面改变为4时，pageMain->raise()不生效
        pageSetting->raise();
        break;
    }


    nRuningAnimationCount--;
}

void MiddleWidget::onReloadMusic(QString musicFileNamePath)
{
    QFileInfo fileinfo(musicFileNamePath);
    onSetMusicTitle(fileinfo.baseName());
    onSetMusicArtist("");                   //重载时，歌手未知

    pagePreviewLyric->setToDefaultAlbumImage();

    pagePreviewLyric->lyricViewer->setMusicPath(musicFileNamePath);

    pageMain->boxPagePreviewLyric->setToDefaultPic();
}

void MiddleWidget::onSetMusicTitle(QString title)
{
    pagePreviewLyric->lyricViewer->setMusicTitle(title);
    pageMain->boxPagePreviewLyric->onSetSongTitle(title);
}

void MiddleWidget::onSetMusicArtist(QString artist)
{
    pageMain->boxPagePreviewLyric->onSetSongArtist(artist);
}

void MiddleWidget::onSetMusicAlbum(QString album)
{

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

